#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <vector>
#include <string>
#include <iostream>

namespace pt = boost::property_tree;

struct holiday {
	std::string strDate;
	int duration;
	int travel;
};

struct person {
	std::string name;
	std::vector<holiday> p_holidays;
};

struct file_reader{
	std::vector<person> persons;
	void load_all();
};
	
void file_reader::load_all(){
}

int main(int argc, char **argv){
	std::string name_xml_file = "./test.xml";
	std::cout << "Open file: " << name_xml_file << std::endl;
	file_reader fr;
	fr.load_all();
}
