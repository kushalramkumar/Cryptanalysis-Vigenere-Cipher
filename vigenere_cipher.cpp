#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>

std::string gCiphertext = "zpgdl rjlaj kpylx zpyyg lrjgd lrzhz qyjzq repvm swrzy rigzh\
zvreg kwivs saolt nliuw oldie aqewf iiykh bjowr hdogc qhkwa\
jyagg emisr zqoqh oavlk bjofr ylvps rtgiu avmsw lzgms evwpc\
dmjsv jqbrn klpcf iowhv kxjbj pmfkr qthtk ozrgq ihbmq sbivd\
ardym qmpbu nivxm tzwqv gefjh ucbor vwpcd xuwft qmoow jipds\
fluqm oeavl jgqea lrkti wvext vkrrg xani";

std::string gCiphertext2 = "GCMMFSHPX MHJAYKGC HTPVBVRK CJBCSMFV ECEEBNQZ YGMKESNK GKTWXXYI\
 UCETLUQH PTLEMJRK ZVCGRWRF PIEMWFCW UTTVBCQB QROFRXBV IHUBQJWT\
 VGYCGSGE GWMWFYSC TCSIMJAW OBJZBWRL YDQBQGCV LGOHMBAZ VDCRAKBK BEMSQLYN PSGLRFVS";

typedef std::map<char, int> FrequencyDistribution;

void printFrequencyDistribution(FrequencyDistribution xFreqTable)
{
    for(FrequencyDistribution::iterator it = xFreqTable.begin(); it != xFreqTable.end(); it++)
    {
        std::cout << char(it->first) << ": " << it->second << std::endl;
    }
}

double indexOfCoincidence(FrequencyDistribution xFreqTable, unsigned int xLength)
{
    double indCo = 0;
    for(FrequencyDistribution::iterator it = xFreqTable.begin(); it != xFreqTable.end(); it++)
    {
        indCo += (it->second)*(it->second - 1);
    }
    indCo *= (double)1/(xLength*(xLength - 1));

    return indCo;
}

double mutualIndexOfCoincidence(FrequencyDistribution s, unsigned int n, FrequencyDistribution t, unsigned int m)
{
    double mutualIndCo = 0;

    for (int i = 97; i <= 122; i++)
    {
        mutualIndCo += s[char(i)]*t[char(i)];
    }
    mutualIndCo *= (double)1/(n*m);

    return mutualIndCo;
}

FrequencyDistribution createFrequencyDistributionTable(std::string xCiphertext)
{
    FrequencyDistribution freqTable;
    for(int i = 97; i <= 122; i++)
    {
        freqTable.insert(std::make_pair<char, int>(char(i), std::count(xCiphertext.begin(), xCiphertext.end(), char(i))));
    }
    return freqTable;
}

std::pair<double, std::vector<std::string>> analyzeFrequencyDistribution(std::string xCiphertext, unsigned int k)
{
    std::pair<double, std::vector<std::string>> freqAnalysis;
    FrequencyDistribution freqTable;
    double avgIndCo = 0;
    std::vector<std::string> vSubstring;
    xCiphertext.erase(remove_if(xCiphertext.begin(), xCiphertext.end(), isspace), xCiphertext.end());
    
    for(unsigned int i = 0; i < k; i++)
    {
        std::string substring;
        for(unsigned int j = i; j < xCiphertext.size(); j += k)
        {
            substring += xCiphertext[j];
        }
        vSubstring.push_back(substring);
        freqTable = createFrequencyDistributionTable(substring);
        avgIndCo += indexOfCoincidence(freqTable, substring.size());
    }
    avgIndCo /= k;

    return std::make_pair(avgIndCo, vSubstring);
}

std::string substringPlusSigma(std::string xInput, unsigned int xSigma)
{
    std::string shiftedString;
    for (unsigned int i = 0; i < xInput.size(); i++)
    {
        if (char(xInput[i]) + xSigma > 122)
        {
            shiftedString += char(97 + char(xInput[i]) + xSigma - 122 - 1);
        }
        else
        {
            shiftedString += char(xInput[i] + xSigma);
        }
    }
    return shiftedString;
}

std::string vigenere_cipher(std::string s1, std::string key)
{
    std::string output;
    for (unsigned int i = 0; i < s1.size(); i++)
    {
        char new_val = s1[i] - key[i % key.size()];
        if (new_val < 0)
        {
            new_val = 123 + new_val;
            output += char(new_val);
        }
        else
        {
            output += char(s1[i] - (key[i % key.size()] - 97));
        }
    }
    return output;
}

int main(int argc, char **argv)
{
    //std::string ciphertext = gCiphertext;
    std::string ciphertext = gCiphertext2;
    std::transform(ciphertext.begin(), ciphertext.end(), ciphertext.begin(), ::tolower);
    std::pair<double, std::vector<std::string>> freqAnalysis;
    int k = 0;
    double avgIndCo = 0;
    while (avgIndCo < 0.061)
    {
        k++;
        freqAnalysis = analyzeFrequencyDistribution(ciphertext, k);
        avgIndCo = freqAnalysis.first;
    }
    
    std::cout << "Key Length: " << k << std::endl;
    for (int l = 0; l < k; l++)
    {
        for (int j = l+1; j < k; j++)
        {
            std::cout << "s" << l << "-s" << j << ":" << std::endl;
            for (int i = 0; i < 26; i++)
            {
                std::string shiftedString = substringPlusSigma(freqAnalysis.second[j], i);
                FrequencyDistribution s, t;
                s = createFrequencyDistributionTable(freqAnalysis.second[l]);
                t = createFrequencyDistributionTable(shiftedString);
                std::cout << "Shift: " << i << " MutIndCo: " << mutualIndexOfCoincidence(s, freqAnalysis.second[l].size(), t, shiftedString.size()) << std::endl;
            }
        }
    }

    /* Solve the equations for betas manually and then run the following code to decrypt your ciphertext!*/
    for (int keyIndex = 0; keyIndex < 25; keyIndex++)
    {
        std::cout << "TRIAL " << keyIndex << std::endl;
        std::cout << "KEY: " << substringPlusSigma("afypvu", keyIndex) << std::endl;
        std::string substring = ciphertext;
        substring.erase(remove_if(substring.begin(), substring.end(), isspace), substring.end());
        
        /* Breaking Ciphertext 1 */
        //std::cout << vigenere_cipher(substring, substringPlusSigma("afzhbkp", keyIndex)) << std::endl;

        /* Breaking Ciphertext 2 */
        std::cout << vigenere_cipher(substring, substringPlusSigma("afypvu", keyIndex)) << std::endl;

        std::cout << std::endl;
    }

    return 0;
}