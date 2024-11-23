#include <rapidcsv.h>

#include <filesystem>
#include <exception>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <format>
#include <string>
#include <set>
#include <map>
//#include <assert.h>

namespace fs = std::filesystem;
namespace rcv = rapidcsv;

constexpr auto UNITS_WEAPON        = "_units_weaponry.csv";
constexpr auto UNITS_WEAPON_SAVE   =  "units_weaponry.csv";
constexpr auto UNITS_WEAPON_RENAME =  "units_weaponry-rename.csv";

constexpr auto ID_POSTFIX_SHORT         = "/short";
constexpr auto ID_POSTFIX_DEFAULT       = "_default";
constexpr auto ID_POSTFIX_DEFAULT_SHORT = "_default/short";

class WtRename{
	rcv::Document units;
	rcv::Document rename;
	std::map<std::string, size_t> unitMap;

public:
	void operator()() {
		if (!fs::exists(UNITS_WEAPON))        { std::cerr << "ERR: file not present near the executable, missing: " << UNITS_WEAPON        << '\n'; throw std::exception("No file"); }
		if (!fs::exists(UNITS_WEAPON_RENAME)) { std::cerr << "ERR: file not present near the executable, missing: " << UNITS_WEAPON_RENAME << '\n'; throw std::exception("No file"); }

		DerenameWeapons();
	}

private:
	void DerenameWeapons() {
		std::cout << "Renaming weapons\n";

		units .Load(UNITS_WEAPON       , rcv::LabelParams(), rcv::SeparatorParams(';', false, true, false, false));
		rename.Load(UNITS_WEAPON_RENAME, rcv::LabelParams(), rcv::SeparatorParams(';', true, true, true, false), rapidcsv::ConverterParams(), rapidcsv::LineReaderParams(true, '#', true));

		{ // Check UNITS_WEAPON_RENAME table for `odd` IDs duplication
			std::set<std::string> oddIds;
			for (size_t r = 0; r < rename.GetRowCount(); r++) {
				const auto &row = rename.GetRow<std::string>(r);
				if (oddIds.contains(row[0])) {
					std::cerr << "ERR: " << UNITS_WEAPON_RENAME << ": odd ID duplication detected: " << row[0] << ";\n";
					throw std::exception("Odd ID duplication");
				}

				oddIds.insert(row[0]);
			}
		}


#if 0 // List all lables
		{ // List all lables
			const auto &labels = units.GetColumnNames();
			std::cout << "Column labels(" << labels.size() << "):";
			for (const auto &label : labels)
				std::cout << " " << label;
			std::cout << "\n";
		}
#endif

		// Map IDs to the rows positions
		std::cout << "Mapping units..." << '\n';
		const size_t initialRowCount = units.GetRowCount();
		for (size_t r = 0; r < units.GetRowCount(); /*r++*/)
		{
			const auto &row = units.GetRow<std::string>(r);

#if 0 // discard ill-formed rows
			bool ill_formed = false;
			//if (row.empty() || row.size() < 2) ill_formed = true;
			//for (const auto &c : row) if (c.size() > 1 && c[0] == '"' && c[1] == ';') { ill_formed = true; break; }
			//for (const auto &c : row) if (c.size() > 253) { ill_formed = true; break; }

			if (ill_formed) {
				std::cout << "NOTICE: ill-formed row: " << r + initialRowCount - units.GetRowCount() + 2 << ": ";
				for (const auto &c : row)
					std::cout << c << ",";
				std::cout << "\n";

				units.RemoveRow(r);
				continue;
			}
#endif

			unitMap[row[0]] = r;
			r++;
		}
		std::cout << "Mapped " << unitMap.size() << " units";
		//std::cout << " (" << initialRowCount - unitMap.size() - 1 << " entries were ill-formed, but don't mind)";
		std::cout << '\n';

		// Process and apply renames
		for (size_t r = 0; r < rename.GetRowCount(); r++) {
			const auto &row = rename.GetRow<std::string>(r);
			if (row.empty()) continue;
			if (row.size() < 2 || row[0].empty() || row[1].empty()) {
				std::cerr << "ERR: " << UNITS_WEAPON_RENAME << ": ill-formed line for renaming #" << r + 2 << " (blank and comment lines affect the row number)\n";
				throw std::exception(std::format("Ill-formed line for renaming #{}",  r + 2).c_str());
			}

			const std::string &odd  = row[0];
			const std::string &orig = row[1];

			Rerename(odd                           , orig                           );
			Rerename(odd + ID_POSTFIX_SHORT        , orig + ID_POSTFIX_SHORT        );
			Rerename(odd + ID_POSTFIX_DEFAULT      , orig + ID_POSTFIX_DEFAULT      );
			Rerename(odd + ID_POSTFIX_DEFAULT_SHORT, orig + ID_POSTFIX_DEFAULT_SHORT);
		}


		units.Save(UNITS_WEAPON_SAVE);
		std::cout << "\nRenaming of weapons finished, results saved to the: " << UNITS_WEAPON_SAVE << "\n";
		std::cout << "Hint: You can use a file (text) campare tool to see the differences between original and de-renamed file\n";
	}

	void Rerename(std::string odd, std::string orig)
	{
		// Wrap IDs into double quotes
		if (!odd .empty() && odd[0]  != '"') odd  = '"' + odd  + '"';
		if (!orig.empty() && orig[0] != '"') orig = '"' + orig + '"';

		size_t oddRow, origRow;
		if (const auto &it = unitMap.find(odd); it != unitMap.end()) oddRow = it->second;
		else {
			if (odd.find("_default") != std::string::npos || odd.find("/short") != std::string::npos) {
					//std::cout << "NOTICE: odd ID for renaming not found, ID: " << odd << '\n';
					return;
			}
			else { std::cerr << "ERR: odd ID for renaming not found, ID: " << odd << '\n'; return; }
		}

		if (const auto &it = unitMap.find(orig); it != unitMap.end()) origRow = it->second;
		else {
			if (orig.find("_default") != std::string::npos || orig.find("/short") != std::string::npos) {
				//std::cout << "NOTICE: orig ID for renaming not found, ID: " << orig << '\n';
				return;
			}
			else { std::cerr << "ERR: orig ID for renaming not found, ID: " << orig << '\n'; return; }
		}

		Rerename(oddRow, origRow);
	}

	void Rerename(const size_t odd, const size_t orig)
	{
		const auto &rowOrig = units.GetRow<std::string>(orig);
		      auto  rowOdd  = units.GetRow<std::string>(odd );

		// Perform the actual replacement of `off` translations with `orig`-inal ones
		size_t col = 1;
		for (col; col < rowOrig.size(); col++)
		{
			if (rowOdd.size() == col)
				rowOdd.push_back(std::string()); // catch with `rowOrig` size - expand for upcoming content

			if (col == 1) std::cout << "TRACE: renaming: " << std::setw(40) << std::left << rowOdd[0] << ": " << rowOdd[col] << " -> " << rowOrig[col] << '\n';

			rowOdd[col] = rowOrig[col];
		}

		// catch with `rowOrig` content - clear odd unreplaced content (leftovers)
		if (col < rowOdd.size())
			for (size_t c = col; c < rowOdd.size(); c++)
				rowOdd[c].clear();

		units.SetRow(odd, rowOdd); // apply the de-renamed values (of all languages)
	}

};

int main()
{
	WtRename rename;

	try {
		rename();
	}
	catch(std::exception &e) { std::cerr << "EXCEP: " << e.what() << '\n'; }

#ifdef NDEBUG
	std::cout << "\nPress any button to exit..."; std::cin.get();
#endif
}
