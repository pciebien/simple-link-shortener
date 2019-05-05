#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <list>

namespace environment {}
namespace tools {}
namespace lsl {}
namespace ui {}
namespace menu_bl {}

using namespace std;
using namespace lsl;

namespace environment
{
    class env_data 
    {
    public:
        env_data()
        {
            webSymbols = new list<string>();
            webSymbols->push_back("http://");
            webSymbols->push_back("https://");
            webSymbols->push_back("www.");
        }

        string getDomainName(bool dotted = false)
        {
            return dotted ? "." + domain : domain;
        }

        string getServiceName(bool dotted = false)
        {
            return dotted ? "." + serviceName : serviceName;
        }

        list<string> *getWebSymbols()
        {
            return webSymbols;
        }

        ~env_data()
        {
            webSymbols->clear();
            delete webSymbols;
        }

    private:
        const string domain = "lsl";
        const string serviceName = "simple";
        list<string> *webSymbols;
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
            for(uint8_t i = 0; i < encodedPartials->size(); ++i)
                encodedIndex += (*encodedPartials)[i];

            delete encodedPartials;
            return encodedIndex;
        }
    };

    class url_normalizer
    {
    public:
        static string process(string url)
        {
            environment::env_data *env = new environment::env_data();
            list<string> *symbols = env->getWebSymbols();

            for(list<std::string>::iterator it = symbols->begin(); it != symbols->end(); ++it)
                url = replace(url, *it, "");
            
            delete env, symbols;
            return url;
        }
    private:
        static string replace(string base, string from, string to)
        {
            if(!from.empty())
                for(size_t pos = 0; (pos = base.find(from, pos)) != string::npos; pos += to.size())
                    base.replace(pos, from.size(), to);
            return base;
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
        
        bool originalExists(string original)
        {
            for(uint8_t i = 0; i < _links->size(); ++i)
            {
                if((*_links)[i]->getOrigin() == original) 
                    return true;
            }
        } 

        string *getOrigins()
        {
            string *pOrigins = new string[_links->size()];

            for(uint8_t i = 0; i < _links->size(); ++i)
                pOrigins[i] = (*_links)[i]->getOrigin();

            return pOrigins;
        }

        string *getShortenedLinks()
        {
            string *pShortenedLinks = new string[_links->size()];

            for(uint8_t i = 0; i < _links->size(); ++i)
                pShortenedLinks[i] = (*_links)[i]->getShortened();

            return pShortenedLinks;
        }

        int count()
        {
            return _links->size();
        }

        void showLinks()
        {
            for (vector<link*>::iterator link = _links->begin(); link != _links->end(); ++link)
            {
                cout << (*link)->getOrigin() << "  " << (*link)->getShortened() << endl;
            }
        }

        void add(string origin, string *shortenedLink)
        {
            environment::env_data *env = new environment::env_data();
            string baseUrl = env->getServiceName() + env->getDomainName(true) + "/";
            *shortenedLink = baseUrl + tools::base_converter::encode((_links->size() + 1) * 1000000);

            link *pLink = new link(origin, *shortenedLink); 
            _links->push_back(pLink);   

            delete env;         
        }

        ~link_collection()
        {
            for (vector<link*>::iterator link = _links->begin(); link != _links->end(); ++link)
                delete *link;
            _links->clear();

            delete _links;
        }

    private:
        vector<link*> *_links;
    };
}

namespace test
{
    void generateLinks(link_collection *links, int linkNumber)
    {
        while(linkNumber > 0)
        {
            string shortenedLink;
            links->add("https://example.com", &shortenedLink);
            --linkNumber;
        }
    }
}

namespace ui
{
    void drawMenu()
    {
        cout << endl << endl;
        cout << "1 - show link collection" << endl;
        cout << "2 - add new link" << endl;
        cout << "3 - exit" << endl;
        cout << "Choose an action (by number): ";
    }

    void clear()
    {
        system("CLS");
    }
}

namespace menu_bl 
{
    void showLinkCollection(link_collection *links)
    {
        links->showLinks();
    }

    void addNewLink(link_collection *links)
    {
        string originLink, shortenedLink;
        cout << "Enter url: ";
        cin >> originLink;
        originLink = tools::url_normalizer::process(originLink);

        if(links->originalExists(originLink))
        {
            cout << "Given link already exists" << endl;
            return;
        }

        links->add(originLink, &shortenedLink);
        cout << "Added successfully: " << shortenedLink << endl;
    }
}

int main()
{
    link_collection *links = new link_collection();  
    int input;

    do
    {
        ui::drawMenu();
        cin >> input;
        ui::clear();

        switch (input)
        {
            case 1:
                menu_bl::showLinkCollection(links);
                break;
            case 2:
                menu_bl::addNewLink(links);
                break;
            case 3:
                input = 0;
                break;
            default:
                continue;
        }
    } while (input != 0);

    delete links;
    return 0;
}