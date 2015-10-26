/**
 * @file	IHttpRequest.cpp
 * @brief	Projekt IPK 1 - Webovy klient
 *
 * @date	2013/02/17
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#include "IHttpRequest.h"

IHttpRequest::IHttpRequest(string rawUri){
    this->pattern = "^(http\\:\\/\\/)?([a-zA-Z0-9\\_\\-\\.]+)(\\:[0-9]+)?\\/?(.*)$";
    this->valid = false;

    this->rawUri = rawUri;
    this->serverSchema = "http://";
    this->serverPort = 80;
    this->serverParam = "/";
    this->serverFile = "index.html";

    this->parseURI();
}

IHttpRequest::~IHttpRequest(){

}

void IHttpRequest::parseURI(){

    this->valid = false;

    regmatch_t pmatch[5];
    regex_t re;
    int status;

    if(regcomp(&re, this->pattern.c_str(), REG_EXTENDED) != 0)
        throw new IException(SYSTEM_CANNOT_COMPILE_REGEX);

    status = regexec(&re, this->rawUri.c_str(), 5, pmatch, 0);

    if(status == REG_NOMATCH){
        regfree(&re);
        //char buf[256];
        //regerror(status, &re, buf, 256);
        //cerr << buf << endl;

        throw new IException(REQUEST_INVALID_URI);

    }
    else{
        //ServerSchema is optional
        if(pmatch[1].rm_so != -1)
            this->serverSchema = this->rawUri.substr(pmatch[1].rm_so, pmatch[1].rm_eo - pmatch[1].rm_so);

        //ServerHost
        this->serverHost = this->rawUri.substr(pmatch[2].rm_so, pmatch[2].rm_eo - pmatch[2].rm_so);

        //ServerPort is optional
        if(pmatch[3].rm_so != -1){
            string tmpPort = this->rawUri.substr(pmatch[3].rm_so, pmatch[3].rm_eo - pmatch[3].rm_so);
            tmpPort.erase(tmpPort.begin());
            istringstream strPort(tmpPort);
            strPort >> this->serverPort;
        }

        //ServerParam
        if(pmatch[4].rm_so != -1)
            this->serverParam += this->rawUri.substr(pmatch[4].rm_so, pmatch[4].rm_eo - pmatch[4].rm_so);

        //ServerFile
        size_t pos = this->serverParam.find_last_of("/");

        if(this->serverParam == "/" || pos == string::npos || (pos + 1) == this->serverParam.size())
            this->serverFile = "index.html";
        else{
            this->serverFile = this->serverParam.substr(pos + 1);
        }

        //If query isset
        if(this->serverFile.find("?") != string::npos)
            this->serverFile = this->serverFile.substr(0, this->serverFile.find("?"));

        if(this->serverFile == "")
            this->serverFile = "index.html";

        this->serverParam = this->encode(this->serverParam);


        //Set rawHost
        if(this->serverPort == 80){
            this->rawHost = this->serverHost;
        }
        else{
            ostringstream strRawHost;
            strRawHost << this->serverHost << ":" << this->serverPort;
            this->rawHost = strRawHost.str();
        }
    }

    regfree(&re);

    this->valid = true;
}

bool IHttpRequest::isValid(){
    return this->valid;
}

string IHttpRequest::getRequest(){
    ostringstream stream;

    stream << "GET " << this->serverParam << " HTTP/1.1\r\n";
    stream << "Host: " << this->rawHost << "\r\n";
    stream << "From: xkolac12@stud.fit.vutbr.cz\r\n";
    stream << "User-Agent: IPK #1 - xkolac12\r\n";
    stream << "Accept-Charset: utf-8\r\n";
    stream << "Accept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/webp, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\n";
    stream << "Cache-Control: no-cache\r\n";
    stream << "\r\n";

    return stream.str();
}

string IHttpRequest::getRawUri(){
    return this->rawUri;
}

string IHttpRequest::getRawHost(){
    return this->rawHost;
}

string IHttpRequest::getServerSchema(){
    return this->serverSchema;
}

string IHttpRequest::getServerHost(){
    return this->serverHost;
}

int IHttpRequest::getServerPort(){
    return this->serverPort;
}

string IHttpRequest::getServerParam(){
    return this->serverParam;
}

string IHttpRequest::getServerFile(){
    return this->serverFile;
}

string IHttpRequest::replaceAll(string data, string search, string replace){
        if(search.empty())
            return data;

        size_t startPos = 0;
        while((startPos = data.find(search, startPos)) != string::npos){
            data.replace(startPos, search.size(), replace);
            startPos += replace.size();
        }

        return data;
}

string IHttpRequest::encode(string data){
    data = this->replaceAll(data, " ", "%20");
	data = this->replaceAll(data, ";", "%3B");

    return data;
}

void IHttpRequest::dumpRequest(){
    cout << endl << "Request" << endl;
    cout << "- rawUri: " << this->rawUri << endl;
    cout << "- rawHost: " << this->rawHost << endl;
    cout << "- serverSchema: " << this->serverSchema << endl;
    cout << "- serverHost: " << this->serverHost << endl;
    cout << "- serverPort: " << this->serverPort << endl;
    cout << "- serverParam: " << this->serverParam << endl;
    cout << "- serverFile: " << this->serverFile << endl;
    cout << "-------------------------------------" << endl;
}
