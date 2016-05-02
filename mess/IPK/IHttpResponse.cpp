/**
 * @file	IHttpResponse.cpp
 * @brief	Projekt IPK 1 - Webovy klient
 *
 * @date	2013/02/13
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

 #include "IHttpResponse.h"

IHttpResponse::IHttpResponse(){
    //HTTP/1.1 200 OK
    this->pattern = "^([a-zA-Z0-9]+)\\/([0-9\\.]+) ([0-9]{3}) (.*)$";

    this->protocol = "(Undefined)";
    this->version = "(Undefined)";
    this->statusCode = 0;
    this->statusMsg = "(Undefined)";
}

IHttpResponse::~IHttpResponse(){
    this->headers.clear();
}

void IHttpResponse::parseHeader(string rawHeader){
    string header = this->trimHeader(rawHeader);

    regmatch_t pmatch[5];
    regex_t re;

    if(regcomp(&re, this->pattern.c_str(), REG_EXTENDED) != 0)
        throw new IException(SYSTEM_CANNOT_COMPILE_REGEX);

    if(regexec(&re, header.c_str(), 5, pmatch, 0) == REG_NOMATCH){
        size_t pos;

        if((pos = header.find(": ")) != string::npos){
            string name = header.substr(0, pos);
            string value = header.substr(pos + 2, header.size() - pos - 2);

            this->headers.insert(pair<string, string>(name, value));
        }
        //Invalid header
        else{
            //cerr << header << endl;
            //throw new IException(RESPONSE_INVALID_HEADER);
        }
    }
    else{
        this->protocol = header.substr(pmatch[1].rm_so, pmatch[1].rm_eo - pmatch[1].rm_so);
        this->version = header.substr(pmatch[2].rm_so, pmatch[2].rm_eo - pmatch[2].rm_so);

        string statusCode = header.substr(pmatch[3].rm_so, pmatch[3].rm_eo - pmatch[3].rm_so);
        istringstream status(statusCode);
        status >> this->statusCode;

        if(pmatch[4].rm_so != -1)
            this->statusMsg = header.substr(pmatch[4].rm_so, pmatch[4].rm_eo - pmatch[4].rm_so);
    }

    regfree(&re);
}

string IHttpResponse::trimHeader(string rawHeader){
    rawHeader.erase(rawHeader.find_last_not_of(" \n\r\t")+1);

    return rawHeader;
}

string IHttpResponse::getHeader(string header){
    return this->headers[header];
}

string IHttpResponse::getProtocol(){
    return this->protocol;
}

string IHttpResponse::getVersion(){
    return this->version;
}

int IHttpResponse::getStatusCode(){
    return this->statusCode;
}

string IHttpResponse::getStatusMsg(){
    return this->statusMsg;
}

bool IHttpResponse::isValid(){
    if(this->statusCode < IHTTP_CONTINUE || this->statusCode > IHTTP_HTTP_VERSION_NOT_SUPPORTED)
        return false;

    //by rfc2616 - must contain Location field
    if((this->statusCode == IHTTP_MOVED_PERMANENTLY || this->statusCode == IHTTP_FOUND) && (this->headers["Location"] == ""))
        return false;

    return true;
}

void IHttpResponse::dumpResponse(){
    cout << endl << "Response" << endl;
    cout << "- protocol: " << this->protocol << endl;
    cout << "- version: " << this->version << endl;
    cout << "- statusCode: " << this->statusCode << endl;
    cout << "- statusMsg: " << this->statusMsg << endl;
    cout << "------------ HEADERS ----------------" << endl;

    for(map<string, string>::iterator p = this->headers.begin(); p != this->headers.end(); p++){
        cout << "`- " << (*p).first << ": " << (*p).second << endl;
    }
    cout << "-------------------------------------" << endl;
}
