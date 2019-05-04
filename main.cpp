#include <iostream>
#include <string>
#include <vector>
#include <cmath>

namespace lsl {}

using namespace std;
using namespace lsl;

namespace environment
{
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
}

namespace lsl
{
    class link
    {
    public:
        link(string origin, int index)
        {
            _origin = origin;
            _shortened = shorten(index);
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

        string shorten(int index)
        {
            string characters = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
            vector<char> *encodedPartials = new vector<char>();

            while (index > 0)
            {
                int val = index % characters.size();
                encodedPartials->push_back(characters[val]);
                index = (int)floor(index / (float)characters.size());
            }

            string encodedIndex = "";
            for(int i = 0; i < encodedPartials->size(); ++i)
                encodedIndex += (*encodedPartials)[i];

            delete encodedPartials;
            return encodedIndex;
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
}

int main()
{
    environment::env_data *env = new environment::env_data();
    link_collection *links = new link_collection();

    delete env, links;
    return 0;
}