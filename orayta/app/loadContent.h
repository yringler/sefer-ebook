#ifndef LOAD_CONTENT_H
#define LOAD_CONTENT_H

#include <string>
#include <locale>
#include "../lib/commentGroup.h"

/*
	Something useful. Two functions in this header are easier to understand
	with this introduction.

	q < b >
	0 1 2 3
	index of quote start marker == 1, length == 3
	there are 4 charachters which need to be erased
	index of start of marker(1) + length of marker(3)
*/

template<typename ContentT, typename CharT>
void loadContent(ContentT & content, const std::basic_string<CharT> & line) {
	content = line;
}


template<typename CharT>
std::basic_string<CharT> removeThroughString(std::basic_string<CharT> & orig
		const std::basic_string<CharT> & str) {
	orig.erase(0, orig.find(str) + str.length());
}

template<typename CharT>
std::basic_string<CharT> extractToString(const std::basic_string<CharT> & orig
		const std::basic_string<CharT> & str) {
	return orig.substr(0, orig.find(str));
}

// Commentary is loaded *way* diffrent than other stuff
// This is *so* ingenuis. (I hope I spelled that right)
template<typename CommentT, CharT>
void loadContent(CommentGroup<CommentT> & commentGroup, const std::basic_string<CharT> & line
	// eg <b>quote1</b>commentcomment<b>quote2</b>commentcomment etc
	// <b> marks the start of the quote, </b> marks the end
		const std::basic_string<CharT> & quote_start_marker,
		const std::basic_string<CharT> & quote_end_marker)
{
	CommentT tmp;
	
	// if theres an introductory part that isn't on a particular quote
	if(line.find(quote_start_marker) > 10) {
		// remove up to but not including start_marker
		tmp.comment = extractToString(line, quote_start_marker);
		commentGroup.push_back(tmp);
	}

	// while there's another comment
	// use end, because I just removed a start, so if there's only one
	// comment, the test will fail immidiately
	while(line.find(quote_end_marker) != std::basic_string<CharT>::npos) {
		removeThroughString(quote_start_marker);
		tmp.on = extractToString(line, quote_end_marker);
		removeThroughString(line, quote_end_marker);

		tmp.comment = extractToString(line, quote_start_marker);
		removeThroughString(line,quote_start_marker);
		commentGroup.push_back(tmp);
	}
}

template <typename CharT>
std::basic_string<CharT> & trimSpace(std::basic_string<CharT> & str) {
	while(std::isspace(str[0], std::locale()))
		str.erase(0,1);
	while(std::isspace(str[str.length()-1], std::locale()))
		str.erase(str.length()-1, 1);
	return str;
}

template<typename CommentT>
void loadContent(CommentGroup<CommentT> & commentGroup, const std::wstring & line)
{
	// the seif katan for mishna berura comes between these two things
	const std::wstring mb_sk_start(L"<span class=\"S0\">")
	const std::wstring mb_sk_end(L"</span>");
	const std::wstring mb_etc(L"וכו");	// vchu

f(line.find(L"<b>") != std::wstring::npos)
		loadContent(commentGroup, line, L"<b>", L"</b>");
	else if(line.find(L"<B>") != std::wstring::npos)
		loadContent(commentGroup, line, L"<B>", L"</B>");
	else if(line.find(L"{") != std::wstring::npos)
		loadContent(commentGroup, line, L"{", L"}");
	else if(line.find(L"-") != std::wstring::npos) {
		// mishna berura has the honour of being processed right here 
		// the comment is added by the next block first, when a seif ka
		commentGroup.back().on = trimSpace(extractToString(line, L'-'));
		// for now I'll just get rid of this one, maybe later I'll
		// clear out the others also. Its just that its *really*
		// important that mishna berura match, because it starts out on
		// a more general div
		commentGroup.back().on = extractToString(commentGroup.back.on,
				mb_etc);
		removeThroughString(line, L'-');
		commentGroup.back().comment += line;	// append, after seif katan
	} else if(line.find(mb_sk_start) != std::wstring::npos) {
		// this line contains the seif katan number for mishna berura
		removeThroughString(line, mb_sk_start);
		ContentT tmp;
		// (don't worry, no spaces between { and }. I checked)
		tmp.comment = extractToString(line, mb_sk_end);
		tmp.comment = L'{' + tmp.comment + L'}'
		commentGroup.push_back(tmp);
	}
}

#endif
