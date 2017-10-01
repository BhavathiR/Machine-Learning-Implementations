#include "node.h"
#include<iostream>
#include <iterator>
#include <math.h>
#include <set>
#include<algorithm>

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

float calc_entropy(int np, int nn)
{
    if(np==0&&nn==0) return 10000;
    if(np==0||nn==0) return 0;
    float ppos=(float)np/(np+nn), pneg=(float)nn/(np+nn);
//    cout<<ppos<<','<<pneg<<',';

    return -ppos* log2(ppos)-pneg* log2(pneg);
}

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
    if(npost==0&&nnegt==0) return 10000;
    if(npost==0||nnegt==0) return 0;
    float ppos=(float)npost/(npost+nnegt), pneg=(float)nnegt/(npost+nnegt);
    entropy= -ppos* log2(ppos)-pneg* log2(pneg);
//    cout<<ppos<<','<<pneg<<',';

    return entropy;
}

void node::split(int in_ite)
{
    ite=in_ite;
//    cout<<" ("<<npost<<","<<nnegt<<") "<<"->";


    if(npost==0)
    {
        split_type_disc=true;
        disc_split_attr="<=50K";
//        cout<<disc_split_attr;
        return;
    }
    else if(nnegt==0)
    {
        split_type_disc=true;
        disc_split_attr=">50K";
//        cout<<disc_split_attr;
        return;
    }

    float ent_disc=calc_split_disc();
    float ent_cont=calc_split_cont();

    if(ent_disc<=ent_cont)
        split_type_disc=true;
    else
        split_type_disc=false;

    if(split_type_disc)
    {
        cout<<disc_split_attr<<": ";
        cont_children.clear();

        for(map<string,node>::iterator it=disc_children.begin(); it!=disc_children.end();++it)
        {
//        cout<<endl<<endl<<"Splitting "<<it->first<<" under "<<split_attr;
            cout<<"\n";

            for(int i=0; i<ite;++i) cout<<"\t";

            cout<<it->first;
            (it->second).split(ite+1);
        }

    }
    else
    {
        cout<<cont_split_attr<<": ";
        disc_children.clear();

        for(map<int,pair<node,node> >::iterator it=cont_children.begin(); it!=cont_children.end();++it)
        {
//        cout<<endl<<endl<<"Splitting "<<it->first<<" under "<<split_attr;
            cout<<"\n";
            for(int i=0; i<ite;++i) cout<<"\t";
            cout<<"<="<<it->first;
            ((it->second).first).split(ite+1);
            cout<<"\n";
            for(int i=0; i<ite;++i) cout<<"\t";
            cout<<">"<<it->first;
            ((it->second).second).split(ite+1);
        }

    }
}



float node:: calc_split_disc( )
{

    int att_num=0;
    float temp_ent=0, child_ent=10000;
    string attr;
    map<string, node> temp_children;
    bitset<15> b=checked;
//    cout<<entropy;

    for(map<string, vector<string> >::iterator it=disc_values.begin(); it!=disc_values.end(); att_num++,it++)
    {
        if(!checked.test(att_num))
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
            temp_children.erase("?");
//            cout<<endl<<attr<<":"<<temp_children.size()<<": ";
            for(vector<string>::iterator jt=(it->second).begin(); jt!=(it->second).end(); ++jt)
            {
//                cout<<(*jt)<<", ";
                if(temp_children.count(*jt))
                {

                    temp_ent+=(temp_children[*jt].calc_entropy())*(temp_children[*jt].npost+temp_children[*jt].nnegt)/(count_known);
                    temp_children[*jt].setbit(b);
                }
            }
//            cout<<temp_ent;
            if(temp_ent<child_ent)
            {
//                cout<<" Selected";
                child_ent=temp_ent;
                disc_children=temp_children;
                disc_split_attr=attr;
            }
        }
    }

//    for(map<string,node>::iterator it=children.begin(); it!=children.end();++it)
//    {
//        cout<<it->first<<", ";
//    }
    return child_ent;
}


float node:: calc_split_cont()
{
  /*          bitset<15> all_cont_checked("011111100000000");
            if((checked & all_cont_checked) ==all_cont_checked)
            {
                cont_children.clear();
                return 100000;
            }*/
    int att_num=8;
    float temp_ent=0, child_ent=100000;
    string attr;
    bitset<15> bits=checked;
    bitset<15> b=checked;
    float split_value;
    bool noise_check=true;
//    cout<<entropy;

    vector<int> attr_pos, attr_neg;
    for(vector<string>::iterator it=cont_values.begin(); it!=cont_values.end(); att_num++,it++)
    {
 //       if(!checked.test(att_num))
        {
            attr=(*it);
            temp_ent=100000;
            b=checked;
            b.set(att_num);
            attr_pos.clear();
            attr_neg.clear();

            for(vector<adult*>::iterator jt=pos_list.begin(); jt!=pos_list.end(); ++jt)
                if((*jt)->ask_cont(attr)!='?')
                    attr_pos.push_back((*jt)->ask_cont(attr));

            for(vector<adult*>::iterator jt=neg_list.begin(); jt!=neg_list.end(); ++jt)
                if((*jt)->ask_cont(attr)!='?')
                    attr_neg.push_back( (*jt)->ask_cont(attr) );

            sort(attr_pos.begin(), attr_pos.end());
            sort(attr_neg.begin(), attr_neg.end());
            float pt, nt;

            for( pt=0,nt=0; pt<attr_pos.size() && nt<attr_neg.size();)
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
//        cout<<attr<< ' '<<child_ent<<' '<<split_value <<' ';
    }

//    cout<<cont_split_attr;
        if(noise_check)
        {
            cont_children.clear();
            cont_split_attr="Noise";
            return 1;
        }
            for(vector<adult*>::iterator jt=pos_list.begin(); jt!=pos_list.end(); ++jt)
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
            (cont_children[split_value].first).setbit(bits);
            (cont_children[split_value].second).setbit(bits);
            return (cont_children[split_value].first).calc_entropy()* (cfp+cfn)/(cfp+cfn+csp+csn)+(cont_children[split_value].second).calc_entropy()* (csp+csn)/(cfp+cfn+csp+csn);
}


void node::clear()
{
    pos_list.clear();
    npost=0;
    neg_list.clear();
    nnegt=0;
}

float node::prune()
{

    float accuracy=0, child_accuracy=0;
    if(split_type_disc)
    {
        if(disc_children.empty())
        {
            if(disc_split_attr=="<=50K")
                return ((float)nnegt)/(npost+nnegt);
            else if(disc_split_attr==">50K")
                return ((float)npost)/(npost+nnegt);
            return 0;
        }
        else
        {
            accuracy=((float)max(npost,nnegt))/(npost+nnegt);

            for(map<string,node>::iterator it=disc_children.begin(); it!=disc_children.end();++it)
                (it->second).clear();
            for(vector<adult*>::iterator jt=pos_list.begin(); jt!=pos_list.end(); ++jt)
                disc_children[(*jt)->ask_disc(disc_split_attr)].add_pos(*jt);
            for(vector<adult*>::iterator jt=neg_list.begin(); jt!=neg_list.end(); ++jt)
                disc_children[(*jt)->ask_disc(disc_split_attr)].add_neg(*jt);
            float count_known=npost+nnegt-disc_children["?"].npost-disc_children["?"].nnegt;
            disc_children.erase("?");

            for(map<string,node>::iterator it=disc_children.begin(); it!=disc_children.end();++it)
                child_accuracy+=(it->second).prune() * ((it->second).npost+(it->second).nnegt)/(count_known);
            if(child_accuracy>accuracy)
                return child_accuracy;
//            cout<<"\nDeleting"<<disc_split_attr;
            disc_children.clear();
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
        if(cont_children.empty())
        {
            if(cont_split_attr=="Noise")
            {
                disc_children.clear();
                split_type_disc=true;
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
            return 0;
        }
        else
        {
            accuracy=((float)max(npost,nnegt))/(npost+nnegt);

            for(map<int,pair<node,node> >::iterator it=cont_children.begin(); it!=cont_children.end();++it)
            {
                ((it->second).first).clear();
                ((it->second).second).clear();
            }

            float split_value=(cont_children.begin())->first;
            for(vector<adult*>::iterator jt=pos_list.begin(); jt!=pos_list.end(); ++jt)
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

            child_accuracy= (cont_children[split_value].first).prune()* (cfp+cfn)/(cfp+cfn+csp+csn)+(cont_children[split_value].second).prune()* (csp+csn)/(cfp+cfn+csp+csn);

            if(child_accuracy>accuracy)
                return child_accuracy;
//            cout<<"\nDeleting"<<cont_split_attr;
            split_type_disc=true;
            cont_children.clear();
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
}

void node::show(int in_ite)
{
    ite=in_ite;
//    cout<<" ("<<npost<<","<<nnegt<<") "<<"->";

    if(split_type_disc)
    {
        cout<<disc_split_attr<<": ";

        for(map<string,node>::iterator it=disc_children.begin(); it!=disc_children.end();++it)
        {
//        cout<<endl<<endl<<"Splitting "<<it->first<<" under "<<split_attr;
            cout<<"\n";

            for(int i=0; i<ite;++i) cout<<"\t";

            cout<<it->first;
            (it->second).show(ite+1);
        }

    }
    else
    {
        cout<<cont_split_attr<<": ";

        for(map<int,pair<node,node> >::iterator it=cont_children.begin(); it!=cont_children.end();++it)
        {
//        cout<<endl<<endl<<"Splitting "<<it->first<<" under "<<split_attr;
            cout<<"\n";
            for(int i=0; i<ite;++i) cout<<"\t";
            cout<<"<="<<it->first;
            ((it->second).first).show(ite+1);
            cout<<"\n";
            for(int i=0; i<ite;++i) cout<<"\t";
            cout<<">"<<it->first;
            ((it->second).second).show(ite+1);
        }

    }
}
