#include <iostream>
#include <string>
#include <vector>

using namespace std;

class env_data 
{
public:
    string getDomainName(bool dotted = false)
    {
        return dotted ? "." + domain : domain;
    }

private:
    const string domain = "sls";
};

class link
{
public:
    link(string origin)
    {
        _origin = origin;
        _shortened = shorten(origin);
    }

    bool originEqual(string link)
    {
        return _origin == link;
    }

    bool shortenedEqual(string link)
    {
        return _shortened == link;
    }

private:
    string _origin;
    string _shortened;

    string shorten(string origin)
    {
        return "";
    }
};

class link_collection
{
public:
    link_collection()
    {
        _links = new vector<link*>();
    }
    
    ~link_collection()
    {
        delete _links;
    }

private:
    vector<link*> *_links;
};

int main()
{
    env_data *env = new env_data();
    link_collection *db = new link_collection();

    cout << env->getDomainName();

    delete env, db;
    return 0;
}