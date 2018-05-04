#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include "lest.hpp"
#include "mini/ini.h"

typedef std::vector<std::string> T_LineData;

//
// test data
//

// properly formed INI
const T_LineData data_INI_well_formed = {
	";string values",
	"[fruit]",
	"apple=good",
	"banana=very good",
	"grape=supreme",
	"orange=fantastic",
	";number values",
	"[vegetables]",
	"garlic=-3",
	"pepper=0.76",
	"pumpkin=-2",
	";booleans",
	"[nuts]",
	"almond=false",
	"walnut=0",
	"peanut=",
	"cashew=no",
	"coconut=yes"
};

// improperly formed INI with plenty of garbage values
const T_LineData data_INI_not_well_formed = {
	"GARBAGE",
	"",
	"; ;; ; ;;;",
	"      ;  string values    ",
	"",
	"[fruit]          ",
	"",
	"",
	"   GARBAGE",
	"apple= good",
	"GARB    AGE    ",
	"banana =           very good",
	"   GARBAGE  ",
	"grape=supreme",
	"",
	"GARBAGE",
	"",
	"orange =fantastic",
	"",
	"",
	"GARBAGE",
	"GARBAGE",
	"",
	"GARBAGE",
	";number values",
	"GARBAGE",
	"[    vegetables    ]  ",
	"GARBAGE",
	"GARBAGE",
	"GARBAGE",
	"GARBAGE",
	"",
	"garlic = -3",
	"GARBAGE",
	"pepper= 0.76",
	"GARBAGE",
	"GARBAGE",
	"pumpkin =-2",
	";booleans",
	"",
	"GARBAGE",
	"               [ nuts ]               ",
	"",
	"",
	"",
	"",
	"",
	"",
	"almond=false",
	"GARBAGE",
	"GARBAGE",
	"  walnut=                  0",
	"  peanut=                    ",
	"cashew                  =no",
	"GARBAGE",
	"",
	"     coconut=   yes"
	"",
	"GARBAGE",
	"GARBAGE",
	"",
	"GARBAGE",
	"GARBAGE",
	"GARBAGE",
};

// empty file
const T_LineData data_INI_empty = {};

// edge case: key value lines without section
const T_LineData data_INI_edge_case_1 = {
	"ignored1=value1",
	"ignored2=value2"
};

// edge case: key value lines without section with proper form following
const T_LineData data_INI_edge_case_2 = {
	"ignored1=value1",
	"ignored2=value2",
	"[data]",
	"proper1=a",
	"proper2=b"
};

// edge case: single empty section
const T_LineData data_INI_edge_case_3 = {
	"[empty]"
};

// edge case: many empty sections
const T_LineData data_INI_edge_case_4 = {
	"[empty1]",
	"[empty2]",
	"[empty3]",
	"[empty4]",
	"[empty5]"
};

// edge case: many empty sections with data in between
const T_LineData data_INI_edge_case_5 = {
	"[empty1]",
	"[empty2]",
	"[notempty]",
	"a=1",
	"b=2",
	"c=3",
	"[empty3]",
	"[empty4]",
	"[empty5]"
};

//
//  filenames
//
const std::string filename_INI_well_formed = "data1.ini";
const std::string filename_INI_not_well_formed = "data2.ini";
const std::string filename_INI_empty = "data3.ini";
const std::string filename_INI_edge_case_1 = "data4.ini";
const std::string filename_INI_edge_case_2 = "data5.ini";
const std::string filename_INI_edge_case_3 = "data6.ini";
const std::string filename_INI_edge_case_4 = "data7.ini";
const std::string filename_INI_edge_case_5 = "data8.ini";

const std::string filename_dummy = "dummy_______filename_______";

//
//  helper functions
//
bool WriteTestINI(std::string const& filename, T_LineData const& lines)
{
	std::ofstream fileWriteStream(filename);
	if (fileWriteStream.is_open())
	{
		if (lines.size())
		{
			auto it = lines.begin();
			for (;;)
			{
				fileWriteStream << *it;
				if (++it == lines.end())
				{
					break;
				}
				fileWriteStream << std::endl;
			}
		}
		return true;
	}
	return false;
}

void OutputData(std::string const& filename, mINI::INIFile& ini)
{
	std::cout << '=' << filename << '=' << std::endl;
	for (auto it = ini.begin(); it != ini.end(); ++it)
	{
		std::cout << it->first << std::endl;
		auto collection = it->second;
		for (auto it2 = collection.begin(); it2 != collection.end(); ++it2)
		{
			auto const& key = it2->first;
			auto const& value = it2->second;
			std::cout << "   " << key << ": " << value << std::endl;
		}
	}
	std::cout << std::endl;
}

//
// test cases
//
const lest::test mINI_tests[] =
{
	CASE("Basic read")
	{
		// read two INI files with differing form and ensure values match
		// expected: A and B data does not differ
		mINI::INIFile iniDataA(filename_INI_well_formed);
		mINI::INIFile iniDataB(filename_INI_not_well_formed);
		// output all data
		OutputData(filename_INI_well_formed, iniDataA);
		OutputData(filename_INI_not_well_formed, iniDataB);
		// check data
		EXPECT(iniDataA.Get("fruit", "apple") == iniDataB.Get("fruit", "apple"));
		EXPECT(iniDataA.Get("fruit", "banana") == iniDataB.Get("fruit", "banana"));
		EXPECT(iniDataA.Get("fruit", "grape") == iniDataB.Get("fruit", "grape"));
		EXPECT(iniDataA.Get("fruit", "orange") == iniDataB.Get("fruit", "orange"));
		EXPECT(iniDataA.GetUInt("vegetables", "garlic") == iniDataB.GetUInt("vegetables", "garlic"));
		EXPECT(fabs(iniDataA.GetFloat("vegetables", "pepper") - iniDataB.GetFloat("vegetables", "pepper")) < 0.1);
		EXPECT(iniDataA.GetInt("vegetables", "pumpkin") == iniDataB.GetInt("vegetables", "pumpkin"));
		EXPECT(iniDataA.GetBool("nuts", "almond") == iniDataB.GetBool("nuts", "almond"));
		EXPECT(iniDataA.GetBool("nuts", "walnut") == iniDataB.GetBool("nuts", "walnut"));
		EXPECT(iniDataA.GetBool("nuts", "peanut") == iniDataB.GetBool("nuts", "peanut"));
		EXPECT(iniDataA.GetBool("nuts", "cashew") == iniDataB.GetBool("nuts", "cashew"));
		EXPECT(iniDataA.GetBool("nuts", "coconut") == iniDataB.GetBool("nuts", "coconut"));
	},

	CASE("Read missing file")
	{
		// attempt to read from file that doesn't exist
		// expected: empty data
		mINI::INIFile iniDataMissingFile(filename_dummy);
		EXPECT(iniDataMissingFile.Size() == 0u);
	},

	CASE("Read an empty file")
	{
		// read from an empty file
		// expected: empty data
		mINI::INIFile iniDataEmpty(filename_INI_empty);
		EXPECT(iniDataEmpty.Size() == 0u);
	},

	CASE("Read edge case files")
	{
		// edge case 1: sectionless key/values
		// expected: empty data
		mINI::INIFile iniEdgeCase1(filename_INI_edge_case_1);
		OutputData(filename_INI_edge_case_1, iniEdgeCase1);
		EXPECT(iniEdgeCase1.Size() == 0u);

		// edge case 2: sectionless key/values at begining, real data following
		// expected: result data only contains real data
		mINI::INIFile iniEdgeCase2(filename_INI_edge_case_2);
		OutputData(filename_INI_edge_case_2, iniEdgeCase2);
		EXPECT(iniEdgeCase2.Size() == 1u);
		EXPECT(iniEdgeCase2.Size("data") == 2u);
		EXPECT(iniEdgeCase2.Get("data", "proper1") == "a");
		EXPECT(iniEdgeCase2.Get("data", "proper2") == "b");

		// edge case 3: single empty section
		// expected: data contains a single empty section
		mINI::INIFile iniEdgeCase3(filename_INI_edge_case_3);
		OutputData(filename_INI_edge_case_3, iniEdgeCase3);
		EXPECT(iniEdgeCase3.Size() == 1u);
		EXPECT(iniEdgeCase3.Size("empty") == 0u);

		// edge case 4: many empty sections
		// expected: data contains five empty sections
		mINI::INIFile iniEdgeCase4(filename_INI_edge_case_4);
		OutputData(filename_INI_edge_case_4, iniEdgeCase4);
		EXPECT(iniEdgeCase4.Size() == 5u);
		EXPECT(iniEdgeCase4.Size("empty1") == 0u);
		EXPECT(iniEdgeCase4.Size("empty2") == 0u);
		EXPECT(iniEdgeCase4.Size("empty3") == 0u);
		EXPECT(iniEdgeCase4.Size("empty4") == 0u);
		EXPECT(iniEdgeCase4.Size("empty5") == 0u);

		// edge case 5: empty sections with data in between
		// expected: 5 empty sections and 1 non-empty section
		mINI::INIFile iniEdgeCase5(filename_INI_edge_case_5);
		OutputData(filename_INI_edge_case_5, iniEdgeCase5);
		EXPECT(iniEdgeCase5.Size() == 6u);
		EXPECT(iniEdgeCase5.Size("empty1") == 0u);
		EXPECT(iniEdgeCase5.Size("empty2") == 0u);
		EXPECT(iniEdgeCase5.Size("empty3") == 0u);
		EXPECT(iniEdgeCase5.Size("empty4") == 0u);
		EXPECT(iniEdgeCase5.Size("empty5") == 0u);
		EXPECT(iniEdgeCase5.Size("notempty") == 3u);
		EXPECT(iniEdgeCase5.Get("notempty", "a") == "1");
		EXPECT(iniEdgeCase5.Get("notempty", "b") == "2");
		EXPECT(iniEdgeCase5.Get("notempty", "c") == "3");
	},

	CASE("Read and check for case insensitivity")
	{
	}
};

int main(int argc, char** argv)
{
	// write test files
	WriteTestINI(filename_INI_well_formed, data_INI_well_formed);
	WriteTestINI(filename_INI_not_well_formed, data_INI_not_well_formed);
	WriteTestINI(filename_INI_empty, data_INI_empty);
	WriteTestINI(filename_INI_edge_case_1, data_INI_edge_case_1);
	WriteTestINI(filename_INI_edge_case_2, data_INI_edge_case_2);
	WriteTestINI(filename_INI_edge_case_3, data_INI_edge_case_3);
	WriteTestINI(filename_INI_edge_case_4, data_INI_edge_case_4);
	WriteTestINI(filename_INI_edge_case_5, data_INI_edge_case_5);
	// run tests
	if (int failures = lest::run(mINI_tests, argc, argv))
	{
		return failures;
	}

	return std::cout << std::endl << "All tests passed!" << std::endl, EXIT_SUCCESS;
}
