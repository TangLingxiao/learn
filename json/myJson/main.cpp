#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <string>
using namespace rapidjson;


void ParseJson(const std::string& json) {
	Document d;
	d.Parse(json.c_str());
	
	std::cout << d["Body"].GetString() << std::endl;
}


std::string BuildJson() {
	Document d;
	d.SetObject();
	
	Value Header;
	Header.SetObject();
	Header.AddMember("Appid", Value().SetString("1413888888"), d.GetAllocator());
	Header.AddMember("SeqId", Value().SetString("AAAAAAAAAAAAAAAAA"), d.GetAllocator());
	Header.AddMember("SendTime", Value().SetString("14179765431"), d.GetAllocator());
	Header.AddMember("ServiceName", Value().SetString("p.s.m"), d.GetAllocator());
	Header.AddMember("SubCmd", Value().SetString("QueryPartion"), d.GetAllocator());
	Header.AddMember("Version", Value().SetString("V1.0"), d.GetAllocator());
	Header.AddMember("Source", Value().SetString("20"), d.GetAllocator());
	Header.AddMember("Serial", Value().SetString("BBBBBBBBBBBBBBB"), d.GetAllocator());
	Header.AddMember("ResultCode", Value().SetString("0"), d.GetAllocator());
	Header.AddMember("RetErrMsg", Value().SetString(""), d.GetAllocator());
	d.AddMember("Header", Header, d.GetAllocator());
	std::string body{"{\"RetCode\":\"0\",\"RetMsg\":\"Success\",\"PartitionListMax\":\"1\",\"PartitionList\":[{\"PartitionId\":\"1\",\"PartitionName\":\"weixinyiqu\"}],\"TotalPageNo\":\"1\"}"};
	d.AddMember("Body", Value().SetString(body.c_str(),body.length()), d.GetAllocator());
	
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	d.Accept(writer);

	std::cout << buffer.GetString() << std::endl;
	return buffer.GetString();
}

int main()
{
	std::cout << std::hex<<int('e') << std::endl;
	ParseJson(BuildJson());
	system("pause");
	return 0;
}