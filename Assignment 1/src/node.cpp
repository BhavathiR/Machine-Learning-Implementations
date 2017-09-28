#include "node.h"
#include<iostream>
#include <iterator>
#include <math.h>
/*
node::node(vector < adult* > plist, vector < adult* > nlist, int p, int n)
{
    pos_list=plist;
    neg_list=nlist;
    checked=0;
    npost=p;
    nnegt=n;
    calc_entropy();
    calc_split();
}
*/
node::node()
{
    checked=0;
    npost=0;
    nnegt=0;
}

node::~node()
{
    //dtor
}

void node::add_pos(adult* ad)
{
    pos_list.push_back(ad);
    npost++;
}

void node::add_neg(adult* ad)
{
    neg_list.push_back(ad);
    nnegt++;
}

void node::setbit(bitset<15> b)
{
    checked=b;
}

float node::calc_entropy()
{
    if(npost==0||nnegt==0) return 0;
    float ppos=(float)npost/(npost+nnegt), pneg=(float)nnegt/(npost+nnegt);
    entropy= -ppos* log2(ppos)-pneg* log2(pneg);
//    cout<<ppos<<','<<pneg<<',';

    return entropy;
}

void node::children_split()
{
    for(map<string,node>::iterator it=children.begin(); it!=children.end();++it)
    {
//        cout<<endl<<endl<<"Splitting "<<it->first<<" under "<<split_attr;
    cout<<"\n";

    for(int i=0; i<ite;++i) cout<<"\t";
            cout<<it->first;
        (it->second).calc_split(ite+1);
    }
}

void node:: calc_split( int in_ite)
{
    ite=in_ite;

    cout<<" ("<<npost<<","<<nnegt<<") "<<"->";
    if(npost==0)
    {
        split_attr="<=50K";
        cout<<split_attr;
        return;
    }
    else if(nnegt==0)
    {
        split_attr=">50K";
        cout<<split_attr;
        return;
    }
    int i=0;
    float temp_ent=0, child_ent=10000;
    string attr;
    map<string, node> temp_children;
    bitset<15> b=checked;
//    cout<<entropy;
    for(map<string, vector<string> >::iterator it=disc_values.begin(); it!=disc_values.end(); i++,it++)
    {
        if(!checked.test(i))
        {
            attr=it->first;
            temp_children.clear();
            temp_ent=0;
            b=checked;
            b.set(i);
            temp_ent=0;
            for(vector<adult*>::iterator jt=pos_list.begin(); jt!=pos_list.end(); ++jt)
                temp_children[(*jt)->ask_disc(attr)].add_pos(*jt);
            for(vector<adult*>::iterator jt=neg_list.begin(); jt!=neg_list.end(); ++jt)
                temp_children[(*jt)->ask_disc(attr)].add_neg(*jt);
            temp_children.erase("?");
//            cout<<endl<<attr<<":"<<temp_children.size()<<": ";
            for(vector<string>::iterator jt=(it->second).begin(); jt!=(it->second).end(); ++jt)
            {
//                cout<<(*jt)<<", ";
                if(temp_children.count(*jt))
                {

                    temp_ent+=(temp_children[*jt].calc_entropy())*(temp_children[*jt].npost+temp_children[*jt].nnegt)/(npost+nnegt);
                    temp_children[*jt].setbit(b);
                }
            }
//            cout<<temp_ent;
            if(temp_ent<child_ent)
            {
//                cout<<" Selected";
                child_ent=temp_ent;
                children=temp_children;
                split_attr=attr;
            }
        }
    }

    cout<<split_attr<<": ";
/*    for(map<string,node>::iterator it=children.begin(); it!=children.end();++it)
    {
        cout<<it->first<<", ";
    }
*/
    children_split();
}

