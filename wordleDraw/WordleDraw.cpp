#include <iostream>
#include <filesystem>
#include <exception>

#include "WordleRestClient.h"
#include "WordleDrawCore.h"

int main()
{
    WordleRestClient client;
    string answer = client.GetCurrentAnswerWord();
    
    
    //TODO ARGS FOR PATH
    std::filesystem::path path("PATH");
    
    //TODO READ FROM FILE
    WordleBitmap bitmap = {
        {
        vector<bool> {1,0,1,0,1},
        vector<bool> {0,1,0,1,0},
        vector<bool> {1,0,1,0,1},
        vector<bool> {0,1,0,1,0},
        vector<bool> {1,0,1,0,1},
        vector<bool> {0,1,0,1,0}
        }
    };

    WordleDrawCore core(path, answer);
    
    //TODO PRETTY OUTPUT
    try
    {
        auto a = core.GetWordsForBitmap(bitmap);
        for (auto word : a)
        {
            std::cout << word << std::endl;
        }
    }
    catch(std::exception ex)
    {
        std::cout << ex.what();
    }

}
