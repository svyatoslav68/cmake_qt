/**** Head file of class condition of graphic holidays ****
*****         Name of file: tcondition.hpp             ****
**********************************************************/
#ifndef TCONDITION_HPP
#define TCONDITION_HPP

struct MemberGroup;
struct ConditionOfGroup;

struct ItemOfGroup {
	ItemOfGroup(std::string _name);
	ItemOfGroup(const ItemOfGroup &);
	~ItemOfGroup();
	std::string name;
	std::vector<MemberGroup>  *children;
	std::vector<ConditionOfGroup> *conditions;
	//std::array<int,366> holidays_in_year {0};
	std::array<std::pair<int, std::vector<int>>,366> holidays_in_year;
};

#endif //TCONDITION_HPP
