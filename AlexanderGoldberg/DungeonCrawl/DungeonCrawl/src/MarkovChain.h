#pragma once
#include <vector>
#include <map>
#include <string>
class MarkovChain
{
protected:
	int m_keyLength = 0;
	struct MarkovUnit {
		std::string ThisPart;
		std::vector<std::string> CanTransitionTo;
		std::string GetRandomElement() 
		{
			if (CanTransitionTo.size() > 0)
			{
				return CanTransitionTo[rand() % CanTransitionTo.size()];
			}
			return "";
		}
	};
	//std::vector<MarkovUnit> m_tuples;
	std::map<std::string, MarkovUnit> MarkovDictionary;
	std::pair<std::string, MarkovUnit> createDicElement(std::string _thisEntry, std::string _convertsTo);
	std::vector<std::string> m_randomGetter;
	void createDictionary(int _keyLength = 3);
	bool contains3orMoreOfSame(std::string& _name);
public:
	void Generate(int _keyLengths);
	std::string GetName(int _length = 6);
	MarkovChain();
	~MarkovChain();
};

