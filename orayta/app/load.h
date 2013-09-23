#ifndef LOAD_H
#define LOAD_H

#include <string>
#include <vector>
#include <fstream>
#include <cassert>
#include "markerLevelTranslator.h"

// Stores what part of markers of each level are used,
// all or just the last section. vector[0] - for level 0, [1] for 1, etc
typedef std::vector<int> Use;
const std::wstring markers; (L"~!$^");
// which part of marked line *to*use*. Default is last.
enum ToUse{last,all};

/*
 * LoadT: what information is beging loaded to. eg a string
 * ToT: where all the LoadTs are being stored. eg a queue (of strings)
 * marker: each thing, eg perek or mishna, has a line preceding
 * 	it with structural information eg perek 1 mishna 2.
 * 	these lines are prefaced by a "marker", one of ~!$^ 
 * 	(and maybe a few others).
 * markered line: line with a marker
 * ToUse/Section: 'markered line's often have multiple sections, where
 * 	the first will be more general and the second more specific,
 * 	eg ~Chapter 10-paragraph 5
 * 	ToUse is an enum which says which section *to use* - both, or 
 * 	only the last, most specific one.
 * 	Default behavior is to use the last one.
 * WithFunc: takes args: LoadT, ifstream
 * 	     returns : 0 on EOF
 */
template <typename LoadT, typename ToT, class WithFunc>
class Loader
{
private:
	Use * use;	// says how to *use* each level of marker
	std::wifstream & stream;
	ToT & data;	// the thing the file is being loaded into
	MarkerLevelTranslator translator;

	// arg is marked line. Replaces it with section lable based on
	// what its supposed to use (ie all or last
	void getLocationLable(std::wstring &, ToUse toUse);
	// returns what parts of marked line to use
	// also adds stuff to translator. sorry
	ToUse getToUse(const std::wstring &);

	// returns zero if reaches EOF
	bool getNextNonBlank(std::wstring &);
	bool getNextMarkered(std::wstring &);

	bool isMarker(wchar_t wc) { 
		return (markers.find(wc) != markers.end());
	}
public:
	Loader(std::wifstream str, ToT & a_data, Use * a_use = 0)
		: stream(str), data(a_data) {
		use = a_use;
	}
	bool load();
};
// list of markers - more?

#endif