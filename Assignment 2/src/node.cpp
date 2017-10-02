#include "node.h"
#include<iostream>
#include <iterator>
#include <math.h>
#include <set>
#include<algorithm>

//Function to return entropy given number of positive and negative samples
float calc_entropy(int np, int nn)
{
    if(np==0&&nn==0) return 10000;
    if(np==0||nn==0) return 0;
    float ppos=(float)np/(np+nn), pneg=(float)nn/(np+nn);
//    cout<<ppos<<','<<pneg<<',';

    return -ppos* log2(ppos)-pneg* log2(pneg);
}

//Constructor
node::node()
{
    checked=0;
    npost=0;
    nnegt=0;
    split_type_disc=true;
    end_of_tree=false;
}

node::~node()
{
    //dtor
}

//Adding positive sample (>50K)
void node::add_pos(adult* ad)
{
    pos_list.push_back(ad);
    npost++;
}

//Adding negative sample (<=50K)
void node::add_neg(adult* ad)
{
    neg_list.push_back(ad);
    nnegt++;
}

//Setting bits for checked attributes
void node::setbit(bitset<15> b)
{
    checked=b;
}

//Removing data from node
void node::clear()
{
    pos_list.clear();
    npost=0;
    neg_list.clear();
    nnegt=0;
}

//Class function for calculating node entropy
float node::calc_entropy()
{
    if(npost==0&&nnegt==0) return 10000;
    if(npost==0||nnegt==0) return 0;
    float ppos=(float)npost/(npost+nnegt), pneg=(float)nnegt/(npost+nnegt);
    entropy= -ppos* log2(ppos)-pneg* log2(pneg);
//    cout<<ppos<<','<<pneg<<',';   //Debugging comment

    return entropy;
}

//Recursive function to calculate class split of decision tree
void node::split(int in_ite)
{
    ite=in_ite;
//    cout<<" ("<<npost<<","<<nnegt<<") "<<"->";    //Debugging Comment


    if(npost==0)    //If no positive examples, class is <=50K
    {
        split_type_disc=true;
        end_of_tree=true;
        disc_split_attr="<=50K";
//        cout<<disc_split_attr;    //Debugging Comment
        return;
    }
    else if(nnegt==0)   //If no negative examples, class is >50K
    {
        split_type_disc=true;
        end_of_tree=true;
        disc_split_attr=">50K";
//        cout<<disc_split_attr;    //Debugging Comment
        return;
    }

    float ent_disc=calc_split_disc();  //Find min entropy of discrete attribute splits
    float ent_cont=calc_split_cont();   //Find min entropy of continuous attribute splits


    if(ent_disc<=ent_cont)  //Choose split with minimum entropy
        split_type_disc=true;
    else
        split_type_disc=false;

    if(split_type_disc)
    {
//        cout<<disc_split_attr<<": ";  //Debugging Comment
        cont_children.clear();

        for(map<string,node>::iterator it=disc_children.begin(); it!=disc_children.end();++it) //Splitting children
        {
/*
            cout<<"\n";
            for(int i=0; i<ite;++i) cout<<"\t";
            cout<<it->first;  */    //Debugging Comment
            (it->second).split(ite+1);
        }

    }
    else
    {
        if(cont_split_attr=="Noise")    //For undetermined leaf nodes, assign class with max probability
        {
            disc_children.clear();
            split_type_disc=true;
            if(npost>nnegt)
            {
                end_of_tree=true;
                disc_split_attr=">50K";
            }
            else
            {
                end_of_tree=true;
                disc_split_attr="<=50K";
            }
            return;
        }
//        cout<<cont_split_attr<<": ";  //Debugging Comment
        disc_children.clear();

        for(map<int,pair<node,node> >::iterator it=cont_children.begin(); it!=cont_children.end();++it)
        {
/*            cout<<"\n";
            for(int i=0; i<ite;++i) cout<<"\t";
            cout<<"<="<<it->first;*/    //Debugging Comment
            ((it->second).first).split(ite+1);
/*            cout<<"\n";
            for(int i=0; i<ite;++i) cout<<"\t";
            cout<<">"<<it->first;   */  //Debugging Comment
            ((it->second).second).split(ite+1);
        }

    }
}


//Calculate best split for discrete attributes
float node:: calc_split_disc( )
{

    int att_num=0;      //Attributes 0 to 7
    float temp_ent=0, child_ent=10000;
    string attr;
    map<string, node> temp_children;
    bitset<15> b=checked;

    for(map<string, vector<string> >::iterator it=disc_values.begin(); it!=disc_values.end(); att_num++,it++)   //Check split for all discrete attributes
    {
        if(!checked.test(att_num))  //If not used already
        {
            attr=it->first;
            temp_children.clear();
            temp_ent=0;
            b=checked;
            b.set(att_num);
            temp_ent=0;
            for(vector<adult*>::iterator jt=pos_list.begin(); jt!=pos_list.end(); ++jt)
                temp_children[(*jt)->ask_disc(attr)].add_pos(*jt);
            for(vector<adult*>::iterator jt=neg_list.begin(); jt!=neg_list.end(); ++jt)
                temp_children[(*jt)->ask_disc(attr)].add_neg(*jt);
            float count_known=npost+nnegt-temp_children["?"].npost-temp_children["?"].nnegt;
            temp_children.erase("?");                                             //Ignoring cases with unknown attribute value ?

            for(vector<string>::iterator jt=(it->second).begin(); jt!=(it->second).end(); ++jt)     //Add weighted entropies of children
            {
                if(temp_children.count(*jt))
                {
                    temp_ent+=(temp_children[*jt].calc_entropy())*(temp_children[*jt].npost+temp_children[*jt].nnegt)/(count_known);
                    temp_children[*jt].setbit(b);
                }
            }
            if(temp_ent<child_ent)      //If attribute split entropy is less than old split entropy, choose
            {
                child_ent=temp_ent;
                disc_children=temp_children;
                disc_split_attr=attr;
            }
        }
    }

    return child_ent;   //Return entropy
}

//Calculate best split for continuous attributes
float node:: calc_split_cont()
{
    int att_num=8;      //Attributes 8 to 13
    float temp_ent=0, child_ent=100000;
    string attr;
    bitset<15> bits=checked;
    bitset<15> b=checked;
    float split_value;
    bool noise_check=true;

    vector<int> attr_pos, attr_neg;
    for(vector<string>::iterator it=cont_values.begin(); it!=cont_values.end(); att_num++,it++) //Check split for all continuous attributes
    {
        if(!checked.test(att_num))  //If not masked
        {
            attr=(*it);
            temp_ent=100000;
//            b=checked;
//            b.set(att_num);
            attr_pos.clear();
            attr_neg.clear();

            for(vector<adult*>::iterator jt=pos_list.begin(); jt!=pos_list.end(); ++jt)
                if((*jt)->ask_cont(attr)!='?')      //Ignoring cases with unknown attribute value ?
                    attr_pos.push_back((*jt)->ask_cont(attr));

            for(vector<adult*>::iterator jt=neg_list.begin(); jt!=neg_list.end(); ++jt)
                if((*jt)->ask_cont(attr)!='?')      //Ignoring cases with unknown attribute value ?
                    attr_neg.push_back( (*jt)->ask_cont(attr) );

            sort(attr_pos.begin(), attr_pos.end());
            sort(attr_neg.begin(), attr_neg.end());
            float pt, nt;

            for( pt=0,nt=0; pt<attr_pos.size() && nt<attr_neg.size();)  //Checking for best split value of attribute
            {
                if(attr_pos[pt]<attr_neg[nt])
                {
                    noise_check=false;
                    while(attr_pos[pt]<attr_neg[nt] && pt<attr_pos.size())
                    {
                        pt++;
                    }
                    temp_ent=(float)((pt+nt)/(attr_pos.size()+attr_neg.size())*(::calc_entropy(pt,nt)) + (attr_pos.size()+attr_neg.size()-pt-nt)/(attr_pos.size()+attr_neg.size())*(::calc_entropy(attr_pos.size()-pt,attr_neg.size()-nt)));
                    if(child_ent>temp_ent)
                    {
                        if((nt==0&&pt==0)||(nt==attr_neg.size()&&pt==attr_pos.size()))
                            noise_check=true;
                        child_ent=temp_ent;
                        cont_split_attr=attr;
                        split_value=(attr_pos[pt-1]+attr_neg[nt])/2;
                        bits=b;
                    }
                }
                else if(attr_pos[pt]>attr_neg[nt])
                {
                    noise_check=false;
                    while(attr_pos[pt]>attr_neg[nt] && nt<attr_neg.size())
                    {
                        nt++;
                    }
                    temp_ent=(float)((pt+nt)/(attr_pos.size()+attr_neg.size())*(::calc_entropy(pt,nt)) + (attr_pos.size()+attr_neg.size()-pt-nt)/(attr_pos.size()+attr_neg.size())*(::calc_entropy(attr_pos.size()-pt,attr_neg.size()-nt)));
                    if(child_ent>temp_ent)
                    {
                        if((nt==0&&pt==0)||(nt==attr_neg.size()&&pt==attr_pos.size()))
                            noise_check=true;
                        child_ent=temp_ent;
                        cont_split_attr=attr;
                        split_value=(attr_pos[pt]+attr_neg[nt-1])/2;
                        bits=b;
                    }
                }
                else if(attr_pos[pt]==attr_neg[nt])
                {
                    while (attr_pos[pt]==attr_neg[nt] && nt<attr_neg.size())
                        nt++;
                    while(attr_pos[pt]==attr_neg[nt-1] && pt<attr_pos.size())
                        pt++;
                    temp_ent=(float)((pt+nt)/(attr_pos.size()+attr_neg.size())*(::calc_entropy(pt,nt)) + (attr_pos.size()+attr_neg.size()-pt-nt)/(attr_pos.size()+attr_neg.size())*(::calc_entropy(attr_pos.size()-pt,attr_neg.size()-nt)));
                    if(child_ent>temp_ent)
                    {
                        if((nt==0&&pt==0)||(nt==attr_neg.size()&&pt==attr_pos.size()))
                            noise_check=true;
                        child_ent=temp_ent;
                        cont_split_attr=attr;
                        if(!(pt==attr_pos.size() && nt==attr_neg.size()))
                        {

                            noise_check=false;
                        if(pt==attr_pos.size())
                            split_value=(attr_pos[pt-1] + attr_neg[nt])/2;

                        else if(nt==attr_neg.size())
                            split_value=(attr_pos[pt-1] + attr_pos[pt])/2;

                        else
                            split_value=(attr_pos[pt-1] + min(attr_pos[pt],attr_neg[nt]))/2;
                        }
                        bits=b;

                    }
                }
            }
        }
    }

        if(noise_check)         //No more split possible
        {
            cont_children.clear();
            cont_split_attr="Noise";
            return 1;
        }

        for(vector<adult*>::iterator jt=pos_list.begin(); jt!=pos_list.end(); ++jt) //Adding positive test cases to children
        {
            if((*jt)->ask_cont(cont_split_attr)!='?')       //Ignoring cases with unknown attribute value ?
            {
                if((*jt)->ask_cont(cont_split_attr)<=split_value)
                    (cont_children[split_value].first).add_pos(*jt);
                else
                    (cont_children[split_value].second).add_pos(*jt);
            }

        }
        for(vector<adult*>::iterator jt=neg_list.begin(); jt!=neg_list.end(); ++jt) //Adding negative test cases to children
        {
            if((*jt)->ask_cont(cont_split_attr)!='?')       //Ignoring cases with unknown attribute value ?
            {
                if((*jt)->ask_cont(cont_split_attr)<=split_value)
                    (cont_children[split_value].first).add_neg(*jt);
                else
                    (cont_children[split_value].second).add_neg(*jt);
            }
        }

        float cfp=(cont_children[split_value].first).npost, cfn=(cont_children[split_value].first).nnegt;
        float csp=(cont_children[split_value].second).npost, csn=(cont_children[split_value].second).nnegt;
        (cont_children[split_value].first).setbit(bits);
        (cont_children[split_value].second).setbit(bits);
        return (cont_children[split_value].first).calc_entropy()* (cfp+cfn)/(cfp+cfn+csp+csn)+(cont_children[split_value].second).calc_entropy()* (csp+csn)/(cfp+cfn+csp+csn);      //Calculating weighted children entropy
}


//Function to prune an existing decision tree using loaded testing data
float node::prune()
{
    float accuracy=0, child_accuracy=0;
    if(split_type_disc)
    {
        if(disc_children.empty())   //If leaf node, check value
        {
            if(disc_split_attr=="<=50K")
                return ((float)nnegt)/(npost+nnegt);
            else if(disc_split_attr==">50K")
                return ((float)npost)/(npost+nnegt);
            cout<<"\nError";        //Debugging Comment
            return 0;
        }
        else
        {
            accuracy=((float)max(npost,nnegt))/(npost+nnegt);       //Node accuracy

            for(map<string,node>::iterator it=disc_children.begin(); it!=disc_children.end();++it)  //Clearing training values
                (it->second).clear();

            for(vector<adult*>::iterator jt=pos_list.begin(); jt!=pos_list.end(); ++jt)     //Adding testing values
                if(disc_children.count((*jt)->ask_disc(disc_split_attr)))
                    disc_children[(*jt)->ask_disc(disc_split_attr)].add_pos(*jt);

            for(vector<adult*>::iterator jt=neg_list.begin(); jt!=neg_list.end(); ++jt)
                if(disc_children.count((*jt)->ask_disc(disc_split_attr)))
                    disc_children[(*jt)->ask_disc(disc_split_attr)].add_neg(*jt);

            float count_known=npost+nnegt-disc_children["?"].npost-disc_children["?"].nnegt;
            disc_children.erase("?");       //Ignoring cases with unknown attribute value ?

            for(map<string,node>::iterator it=disc_children.begin(); it!=disc_children.end();++it)  //Weighted sums of children accuracy
                child_accuracy+=(it->second).prune() * ((it->second).npost+(it->second).nnegt)/(count_known);
            if(child_accuracy>accuracy)     //If accuracy after splitting is greater than node accuracy, retain split
                return child_accuracy;
//            cout<<"\nDeleting"<<disc_split_attr;      //Debugging Comment
            disc_children.clear();      //If accuracy after splitting is less than node accuracy, discard split and determine most probable class
            if(npost>nnegt)
            {
                disc_split_attr=">50K";
                return ((float)npost)/(npost+nnegt);
            }
            else
            {
                disc_split_attr="<=50K";
                return ((float)nnegt)/(npost+nnegt);
            }
        }
    }
    else
    {
        accuracy=((float)max(npost,nnegt))/(npost+nnegt);   //Node accuracy

        for(map<int,pair<node,node> >::iterator it=cont_children.begin(); it!=cont_children.end();++it) //Clearing training values
        {
            ((it->second).first).clear();
            ((it->second).second).clear();
        }

        float split_value=(cont_children.begin())->first;
        for(vector<adult*>::iterator jt=pos_list.begin(); jt!=pos_list.end(); ++jt)     //Adding testing values
        {
            if((*jt)->ask_cont(cont_split_attr)!='?')
            {
                if((*jt)->ask_cont(cont_split_attr)<=split_value)
                    (cont_children[split_value].first).add_pos(*jt);
                else
                    (cont_children[split_value].second).add_pos(*jt);
            }

        }
        for(vector<adult*>::iterator jt=neg_list.begin(); jt!=neg_list.end(); ++jt)
        {
            if((*jt)->ask_cont(cont_split_attr)!='?')
            {
                if((*jt)->ask_cont(cont_split_attr)<=split_value)
                    (cont_children[split_value].first).add_neg(*jt);
                else
                    (cont_children[split_value].second).add_neg(*jt);
            }
        }

        float cfp=(cont_children[split_value].first).npost, cfn=(cont_children[split_value].first).nnegt;
        float csp=(cont_children[split_value].second).npost, csn=(cont_children[split_value].second).nnegt;

        child_accuracy= (cont_children[split_value].first).prune()* (cfp+cfn)/(cfp+cfn+csp+csn)+(cont_children[split_value].second).prune()* (csp+csn)/(cfp+cfn+csp+csn);       //Weighted child accuracy

        if(child_accuracy>accuracy)     //If accuracy after splitting is greater than node accuracy, retain split
            return child_accuracy;
//            cout<<"\nDeleting"<<cont_split_attr;      //Debugging Comment
        split_type_disc=true;
        cont_children.clear();      //If accuracy after splitting is less than node accuracy, discard split and determine most probable class
        if(npost>nnegt)
        {
            disc_split_attr=">50K";
            return ((float)npost)/(npost+nnegt);
        }
        else
        {
            disc_split_attr="<=50K";
            return ((float)nnegt)/(npost+nnegt);
        }
    }
}


//Function to print the current tree
void node::show(int in_ite)
{
    ite=in_ite;

    if(split_type_disc)
    {
        cout<<disc_split_attr;
        for(map<string,node>::iterator it=disc_children.begin(); it!=disc_children.end();++it)
        {
            cout<<"\n";
            for(int i=0; i<ite;++i) cout<<"\t";
            cout<<it->first<<": ";
            (it->second).show(ite+1);
        }
    }
    else
    {
        cout<<cont_split_attr;
        for(map<int,pair<node,node> >::iterator it=cont_children.begin(); it!=cont_children.end();++it)
        {
            cout<<"\n";
            for(int i=0; i<ite;++i) cout<<"\t";
            cout<<"  <="<<it->first<<": ";
            ((it->second).first).show(ite+1);
            cout<<"\n";
            for(int i=0; i<ite;++i) cout<<"\t";
            cout<<"  >"<<it->first<<": ";
            ((it->second).second).show(ite+1);
        }
    }
}

//Testing a tree to return the class of an adult
bool node::test(adult ad)
{
    if(end_of_tree)
    {
        if(disc_split_attr=="<=50K")
            return false;
        else if(disc_split_attr==">50K")
            return true;
        cout<<"Error 3";        //Debugging Comment
        return false;
    }
    if(split_type_disc)
    {
//        cout<<endl<<disc_split_attr;      //Debugging Comment
        if(disc_children.empty())
            cout<<"Error 1";        //Debugging Comment
        if(disc_children.count(ad.ask_disc(disc_split_attr)))
            return disc_children[ad.ask_disc(disc_split_attr)].test(ad);
        else return false;
    }
    else
    {
//        cout<<endl<<cont_split_attr;      //Debugging Comment
        if(cont_children.empty())
            cout<<"Error 2";        //Debugging Comment
        if(ad.ask_cont(cont_split_attr)<=(cont_children.begin())->first)
            return (cont_children.begin())->second.first.test(ad);
        else return (cont_children.begin())->second.second.test(ad);
    }
}
