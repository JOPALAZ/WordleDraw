#include <iostream>
#include <filesystem>
#include <exception>

#include "WordleRestClient.h"
#include "WordleDrawCore.h"

int main()
{
    WordleRestClient client;
    string answer = client.GetCurrentAnswerWord();
    
    std::filesystem::path path("valid-wordle-words.txt");
    
    //TODO READ FROM FILE
    WordleBitmap bitmap = {
        {
        std::vector<bool> {1,0,1,0,1},
        std::vector<bool> {0,1,0,1,0},
        std::vector<bool> {1,0,1,0,1},
        std::vector<bool> {0,1,0,1,0},
        std::vector<bool> {1,0,1,0,1},
        std::vector<bool> {0,1,0,1,0}
        }
    };

    char colors[3] {'B','Y','G'};

    WordleDrawCore core(path);
    
    //TODO PRETTY OUTPUT
    try
    {
        auto a = core.GetWordsForBitmap(bitmap, answer);
        for (auto& word : a)
        {
            std::cout << word << std::endl;
        }
        auto b = core.GetColorsForWordAndAnswer(answer, a);

        std::cout << std::endl;

        for (auto& colorLine : b)
        {
            for(auto& color : colorLine )
            {
                std::cout << colors[static_cast<int>(color)];
            }
            std::cout << std::endl;
        }
    }
    catch(std::exception ex)
    {
        std::cout << ex.what();
    }

}
