#pragma once
#include <vector>
#include <list>
#include <string>
#include <iostream>
using namespace std;

typedef vector<bool> blist;
typedef unsigned int uint;

class gate
{
protected:
    string name;
    uint inputNum;
    uint outputNum=1;
public:
    virtual blist realCalu(blist par)=0;
    gate(string name,uint inputNum, uint outputNum=1) :
        name(name), inputNum(inputNum), outputNum(outputNum) {}
    uint getInputNum() { return inputNum; }
    uint getOutputNum() { return outputNum; }
    string getName() { return this->name; }

    blist calu(blist par)
    {
        if(this->inputNum!=par.size())
            throw string("The number of parameters does not match");
        else
            return realCalu(par);
    }
};


class andGate : public gate
{
public:
    andGate() : gate("and",2) {}

    virtual blist realCalu(blist par)
    {
        blist result;
        result.push_back(par[0]&&par[1]);
        return result;
    }
};


class orGate : public gate
{
public:
    orGate() : gate("or",2) {}

    virtual blist realCalu(blist par)
    {
        blist result;
        result.push_back(par[0]||par[1]);
        return result;
    }
};


class notGate : public gate
{
public:
    notGate() : gate("not",1) {}

    virtual blist realCalu(blist par)
    {
        par[0]=!par[0];
        return par;
    }
};


class NAGate : public gate
{
public:
    NAGate() : gate("NA",2) {}

    virtual blist realCalu(blist par)
    {
        andGate g1;
        notGate g2;
        par=g1.calu(par);
        return g2.calu(par);
    }
};


class NOGate : public gate
{
public:
    NOGate() : gate("NO",2) {}

    virtual blist realCalu(blist par)
    {
        orGate g1;
        notGate g2;
        par=g1.calu(par);
        return g2.calu(par);
    }
};

class NOAGate : public gate
{
public:
    NOAGate() : gate("NOA",4) {}

    virtual blist realCalu(blist par)
    {
        andGate a1;
        andGate a2;
        orGate g1;
        notGate g2;

        blist par1;
        par1.push_back(par[0]);
        par1.push_back(par[1]);
        blist par2;
        par2.push_back(par[2]);
        par2.push_back(par[3]);

        //长度都为1
        par1=a1.calu(par1);
        par2=a2.calu(par2);

        blist r;
        r.push_back(par1[0]);
        r.push_back(par2[0]);

        r=g1.calu(r);
        return g2.calu(r);
    }
};

class xorGate : public gate
{
public:
    xorGate() : gate("Xor",2) {}

    virtual blist realCalu(blist par)
    {
        blist result;
        if(par[0]==par[1])
            result.push_back(1);
        else
            result.push_back(0);
        return result;
    }
};

class RSTri : public gate
{
private:
    blist sta;

public:
    RSTri() : gate("RS",2,2)
    {
        sta.push_back(0);
        sta.push_back(1);
    }
    void setQ(bool q)
    {
        sta[Q]=q;
        sta[Q2]=!q;
    }
    bool getQ() { return sta[Q]; }

    const uint R=0;
    const uint S=1;

    const uint Q=0;
    const uint Q2=1;

    virtual blist realCalu(blist par) //第一个是r第二个是s。返回值也是对应，第一个Q2第二个Q
    {

        if(par[R]==0&&par[S]==0) {} //结果不确定
        else if(par[R]==0&&par[S]==1)
        {
            setQ(0);
        }
        else if(par[R]==1&&par[S]==0)
        {
            setQ(1);
        }
        return sta;
    }
};

class RSCTri : public gate
{
private:
    RSTri rstg;

public:
    RSCTri() : gate("RSC",3,2) {}
    void setQ(bool q) { rstg.setQ(q); }
    bool getQ() { return rstg.getQ(); }

    const uint R=0;
    const uint S=1;
    const uint CP=2;

    virtual blist realCalu(blist par)
    {
        blist r;
        if(par[CP]==1)
        {
            //按书上电路，连的是与非门，所以这里取非
            r.push_back(!par[R]);
            r.push_back(!par[S]);
        }
        else
        {
            r.push_back(0);
            r.push_back(0);
        }
        return rstg.calu(r);
    }
};

class DTri : public gate
{
private:
    blist sta;

public:
    DTri() : gate("D",2)
    {
        sta.push_back(0);
    }

    const uint D=0;
    const uint CP=1;

    virtual blist realCalu(blist par)
    {
        if(par[CP]==1)
            sta[D]=par[D];
        return sta;
    }
};

class JKTri : public gate
{
private:
    blist sta;

public:
    JKTri() : gate("JK",3,2)
    {
        sta.push_back(0);
        sta.push_back(1);
    }
    void setQ(bool q)
    {
        sta[Q]=q;
        sta[Q2]=!q;
    }
    bool getQ() { return sta[Q]; }

    const uint J=0;
    const uint K=1;
    const uint CP=2;

    const uint Q=0;
    const uint Q2=1;

    virtual blist realCalu(blist par)
    {
        if(par[CP]==1)
        {
            if(par[J]==0&&par[K]==1)
            {
                setQ(0);
            }
            else if(par[J]==1&&par[K]==0)
            {
                setQ(1);
            }
            else if(par[J]==1&&par[K]==1)
            {
                setQ(!par[Q]);
            }
        }
        return sta;
    }
};

class TTri : public gate
{
private:
    blist sta;

public:
    JKTri() : gate("T",1,2)
    {
        sta.push_back(0);
        sta.push_back(1);
    }
    void setQ(bool q)
    {
        sta[Q]=q;
        sta[Q2]=!q;
    }
    bool getQ() { return sta[Q]; }
	
	const uint T=0;
	
	const uint Q=0;
    const uint Q2=1;
	
	virtual blist realCalu(blist par)
    {
        if(par[CP]==1)
        {
            if(par[T]==1)
            {
                setQ(!par[Q]);
            }
        }
        return sta;
    }
};
