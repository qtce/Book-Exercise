//
// Chapter16 Exercise22
//

#ifndef EX27_H
#define EX27_H

#include <vector>
#include <map>
#include <set>
#include <memory>
#include <fstream>
#include <iostream>
#include <sstream>
#include "debugdelete.h"

class QueryResult;

class TextQuery {
public:
	using line_no = std::vector<std::string>::size_type;
	TextQuery(std::ifstream &);
	QueryResult query(const std::string&) const;
	
private:
	std::shared_ptr<std::vector<std::string>, DebugDelete> file;
	std::map<std::string, std::shared_ptr<std::set<line_no>>, DebugDelete> wm;
};


class QueryResult {
friend std::ostream &print(std::ostream&, const QueryResult&);

public:
	QueryResult(std::string s,
				std::shared_ptr<std::set<TextQuery::line_no>, DebugDelete> p,
				std::shared_ptr<std::vector<std::string>, DebugDelete> f):
		sought(s), lines(p), file(f) { }
		
private:
	std::string sought;
	std::shared_ptr<std::set<TextQuery::line_no>, DebugDelete> lines;
	std::shared_ptr<std::vector<std::string>, DebugDelete> file;
};

std::ostream& print(std::ostream&, const QueryResult& );

TextQuery::TextQuery(std::ifstream &is): file(new std::vector<std::string>)
{
	std::string text;
	while(getline(is, text)){
		file->push_back(text);
		int n = file->size() - 1;
		std::istringstream line(text);
		std::string word;
		while(line >> word) {
			auto &lines = wm[word];
			if(!lines)
				lines.reset(new std::set<line_no>);
			lines->insert(n);
		}		
	}
}

QueryResult
TextQuery::query(const std::string &sought) const
{
	static std::shared_ptr<std::set<line_no>, DebugDelete> nodata(new std::set<line_no>, DebugDelete());
	
	auto loc = wm.find(sought);
	if (loc == wm.end())
		return QueryResult(sought, nodata, file);
	else
		return QueryResult(sought, loc->second, file);
}

std::string make_plural(size_t ctr, const std::string &word,
									const std::string &ending)
{
	return (ctr > 1) ? word + ending : word;
}

std::ostream &print(std::ostream &os, const QueryResult &qr)
{
	os << qr.sought << "occurs" << qr.lines->size() << " "
		<< make_plural(qr.lines->size(), "times", "s") << std::endl;
	
	for(auto num : *qr.lines)
		os << "\t(line " << num + 1 << " ) "
			<< *(qr.file->begin() + num) << std::endl;
	
	return os;
}

#endif