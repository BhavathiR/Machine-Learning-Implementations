#include "node.h"

#include <iterator>
#include <math.h>
#include<iostream>

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
    float ppos=npost/(npost+nnegt), pneg=nnegt/(npost+nnegt);
    entropy= -ppos* log2(ppos)-pneg* log2(pneg);

    return entropy;
}

void node::children_split()
{
    for(map<string,node>::iterator it=children.begin(); it!=children.end();++it)
        (it->second).calc_split();
}

void node:: calc_split()
{
    if(npost==0)
    {
        split_attr="<=50K";
        return;
    }
    else if(nnegt==0)
    {
        split_attr=">50K";
        return;
    }
    int i=0;
    float temp_ent=0, child_ent=entropy;
    string attr;
    map<string, node> m;
    bitset<15> b=checked;
    for(map<string, vector<string> >::iterator it=disc_attributes.begin(); it!=disc_attributes.end(); i++,it++)
    {
        if(!checked.test(i))
        {
            attr=it->first;
            m.clear();
            b=checked;
            b.set(i);
            temp_ent=0;
            for(vector<adult*>::iterator jt=pos_list.begin(); jt!=pos_list.end(); ++jt)
                m[(*jt)->ask_disc(attr)].add_pos(*jt);
            for(vector<adult*>::iterator jt=neg_list.begin(); jt!=neg_list.end(); ++jt)
                m[(*jt)->ask_disc(attr)].add_neg(*jt);
            m.erase("?");
            for(vector<string>::iterator jt=(it->second).begin(); jt!=(it->second).end(); ++jt)
            {
                if(m.count(*jt))
                {
                    temp_ent+=m[*jt].calc_entropy();
                    m[*jt].setbit(b);
                }
            }
            if(temp_ent<child_ent)
            {
                child_ent=temp_ent;
                children=m;
                split_attr=attr;
            }
        }
    }

    cout<<endl<<split_attr<<":\t";
    for(map<string,node>::iterator it=children.begin(); it!=children.end();++it)
    {
        cout<<it->first<<", ";
    }
    children_split();
}

