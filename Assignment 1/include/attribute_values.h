#ifndef ATTRIBUTE_VALUES_H
#define ATTRIBUTE_VALUES_H

#include<vector>
#include<map>
#include<string>
#include<utility>

using namespace std;

const static vector<string> temp_workclass= {"Private", "Self-emp-not-inc", "Self-emp-inc", "Federal-gov", "Local-gov", "State-gov", "Without-pay", "Never-worked"};
const static vector<string> temp_education= {"Bachelors", "Some-college", "11th", "HS-grad", "Prof-school", "Assoc-acdm", "Assoc-voc", "9th", "7th-8th", "12th", "Masters", "1st-4th", "10th", "Doctorate", "5th-6th", "Preschool"};
const static vector<string> temp_marital_status= {"Married-civ-spouse", "Divorced", "Never-married", "Separated", "Widowed", "Married-spouse-absent", "Married-AF-spouse"};
const static vector<string> temp_occupation= {"Tech-support", "Craft-repair", "Other-service", "Sales", "Exec-managerial", "Prof-specialty", "Handlers-cleaners", "Machine-op-inspct", "Adm-clerical", "Farming-fishing", "Transport-moving", "Priv-house-serv", "Protective-serv", "Armed-Forces"};
const static vector<string> temp_relationship= {"Wife", "Own-child", "Husband", "Not-in-family", "Other-relative", "Unmarried"};
const static vector<string> temp_race= {"White", "Asian-Pac-Islander", "Amer-Indian-Eskimo", "Other", "Black"};
const static vector<string> temp_sex= {"Female", "Male"};
const static vector<string> temp_native_country= {"United-States", "Cambodia", "England", "Puerto-Rico", "Canada", "Germany", "Outlying-US(Guam-USVI-etc)", "India", "Japan", "Greece", "South", "China", "Cuba", "Iran", "Honduras", "Philippines", "Italy", "Poland", "Jamaica", "Vietnam", "Mexico", "Portugal", "Ireland", "France", "Dominican-Republic", "Laos", "Ecuador", "Taiwan", "Haiti", "Columbia", "Hungary", "Guatemala", "Nicaragua", "Scotland", "Thailand", "Yugoslavia", "El-Salvador", "Trinadad&Tobago", "Peru", "Hong", "Holand-Netherlands"};
static map <string, vector<string> > disc_values = { make_pair("workclass", temp_workclass),
                                make_pair("education",temp_education),
                                make_pair("marital-status", temp_marital_status),
                                make_pair("occupation", temp_occupation),
                                make_pair("relationship", temp_relationship),
                                make_pair("race", temp_race),
                                make_pair("sex", temp_sex),
                                make_pair("native-country", temp_native_country)};
static map<string,bool> dclassify = {make_pair(">50K",true),make_pair("<=50K",false)};
static vector <string> cont_values = {"age", "education-num", "capital-gain", "capital-loss", "hours-per-week"};

#endif // ATTRIBUTE_VALUES_H_INCLUDED
