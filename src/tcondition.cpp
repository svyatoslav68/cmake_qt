/**** Head file of class condition of graphic holidays ****
*****         Name of file: tcondition.hpp             ****
**********************************************************/

#include <string>
#include <utility>
#include <vector>
#include <array>
#include "modelMembersGroup.hpp"
#include "tcondition.hpp"
#include "cl_parametrs.hpp"

extern clParametrs appParametrs;

ItemOfGroup::ItemOfGroup(std::string _name):name(_name){
		children = new std::vector<MemberGroup>;
		conditions = new std::vector<ConditionOfGroup>;
		for (auto pair_content : holidays_in_year) {
			pair_content.first = 0;
		}
}

ItemOfGroup::ItemOfGroup(const ItemOfGroup &group){
	name = group.name;
	children = new std::vector<MemberGroup>(group.children->begin(), group.children->end());
	conditions = new std::vector<ConditionOfGroup>(group.conditions->begin(), group.conditions->end());
	//copy(group.children->begin(), group.children->end(), children->begin());
	/*for (auto pair_content : holidays_in_year) {
		pair_content.first = 0;
	}*/
}

ItemOfGroup::~ItemOfGroup(){
	delete conditions;
	delete children;
}
