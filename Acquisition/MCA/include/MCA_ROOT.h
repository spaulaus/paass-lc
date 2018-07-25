/// @file MCA_ROOT.h
/// @brief Class to handle outputting MCA data into ROOT files
/// @author K. Smith, C. R. Thornsberry, S. V. Paulauskas
/// @date Updated July 25, 2018
#ifndef PAASSLC_MCA_ROOT_H
#define PAASSLC_MCA_ROOT_H
#include "MCA.h"

#include <iostream>
#include <map>

#include <cstdio>

#include <unistd.h>

class TFile;
class TH1F;

class MCA_ROOT : public MCA {
public:
    ///Default constructor
    MCA_ROOT(AcquisitionInterface *pif, const char *basename);

    ///Defaul destructor
    ~MCA_ROOT();

    bool StoreData(int mod, int ch);

    void Flush();

    bool OpenFile(const char *basename);

    TH1F *GetHistogram(int mod, int ch);

    void Reset();

private:
    TFile *_file;
    std::map<int, TH1F *> _histograms;

    ///A class to handle redirecting stderr
    /**The class redirects stderr to a text file and also saves output in case the user
     * would like to print it. Upon destruction stderr is restored to stdout.
     */
    class cerr_redirect {
    private:
        char buf[BUFSIZ];

    public:
        cerr_redirect(const char *logFile) {
            for (int i = 0; i < BUFSIZ; i++)
                buf[i] = '\0';
            if(!freopen(logFile, "a", stderr))
                std::cerr << "cerr_redirect::cerr_redirect(logFile) - Couldn't reopen the stream!" << std::endl;
            setbuf(stderr, buf);
        };

        /**stderr is pointed to a duplicate of stdout, buffering is then set to no buffering.
         */
        ~cerr_redirect() {
            dup2(fileno(stdout), fileno(stderr));
            setvbuf(stderr, NULL, _IONBF, BUFSIZ);
        };

        void Print() {
            fprintf(stdout, "%s", buf);
            fflush(stdout);
        }

    };
};

#endif
