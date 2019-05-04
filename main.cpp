#include <iostream>
#include <string>
#include <vector>
#include <cmath>

namespace lsl {}
namespace environment {}
namespace tools {}

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

namespace tools
{
    class base_converter
    {
    public:
        static string encode(int value)
        {
            string characters = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
            vector<char> *encodedPartials = new vector<char>();

            while (value > 0)
            {
                int val = value % characters.size();
                encodedPartials->push_back(characters[val]);
                value = (int)floor(value / (float)characters.size());
            }

            string encodedIndex = "";
            for(int i = 0; i < encodedPartials->size(); ++i)
                encodedIndex += (*encodedPartials)[i];

            delete encodedPartials;
            return encodedIndex;
        }
    };
}

namespace lsl
{
    class link
    {
    public:
        link(string origin, string shortened)
        {
            _origin = origin;
            _shortened = shortened;
        }

        bool originEqual(string link)
        {
            return _origin == link;
        }

        bool shortenedEqual(string link)
        {
            return _shortened == link;
        }

        string getOrigin()
        {
            return _origin;
        }

        string getShortened()
        {
            return _shortened;
        }

    private:
        string _origin;
        string _shortened;
    };

    class link_collection
    {
    public:
        link_collection()
        {
            _links = new vector<link*>();
        }
        
        string *getOrigins()
        {
            string *pOrigins = new string[_links->size()];

            for(int i = 0; i < _links->size(); ++i)
                pOrigins[i] = (*_links)[i]->getOrigin();

            return pOrigins;
        }

        string *getShortenedLinks()
        {
            string *pShortenedLinks = new string[_links->size()];

            for(int i = 0; i < _links->size(); ++i)
                pShortenedLinks[i] = (*_links)[i]->getShortened();

            return pShortenedLinks;
        }

        void add(string origin, string *shortenedLink)
        {
            (*shortenedLink) = tools::base_converter::encode(_links->size() + 1);
            link *pLink = new link(origin, (*shortenedLink)); 
            _links->push_back(pLink);            
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

    string shortenedLink;
    links->add("https://google.com", &shortenedLink);
    
    string *pOrigins = links->getOrigins();
    string *pShortenedLinks = links->getShortenedLinks();
    
    for(int i = 0; i < pOrigins->size(); ++i)
    {
        cout << (*pOrigins)[i] << "    " << (*pShortenedLinks)[i] << endl;
    }

    delete env, links, pOrigins, pShortenedLinks;
    return 0;
}