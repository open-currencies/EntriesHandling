
Release: MKDIR_Release_src TNtrNr Util Entry Type1Entry Type2Entry Type3Entry Type4Entry Type5Or15Entry Type5Entry Type6Entry Type7Entry Type8Entry Type9Entry Type10Entry Type11Entry Type12Entry Type13Entry Type14Entry Type15Entry CompleteID CIDsSet ContactInfo BigInt RefereeInfo NotaryInfo libEntriesHandling.a

CXX = g++

MKDIR_Release_src:
	mkdir -p obj/Release/src

TNtrNr: src/TNtrNr.cpp include/TNtrNr.h
	$(CXX) -Wall -Iinclude -c src/$@.cpp -o obj/Release/src/$@.o -std=c++11

Util: src/Util.cpp include/Util.h
	$(CXX) -Wall -Iinclude -c src/$@.cpp -o obj/Release/src/$@.o -std=c++11

Entry: src/Entry.cpp include/Entry.h
	$(CXX) -Wall -Iinclude -c src/$@.cpp -o obj/Release/src/$@.o -std=c++11

Type1Entry: src/Type1Entry.cpp include/Type1Entry.h
	$(CXX) -Wall -Iinclude -c src/$@.cpp -o obj/Release/src/$@.o -std=c++11

Type2Entry: src/Type2Entry.cpp include/Type2Entry.h
	$(CXX) -Wall -Iinclude -c src/$@.cpp -o obj/Release/src/$@.o -std=c++11

Type3Entry: src/Type3Entry.cpp include/Type3Entry.h
	$(CXX) -Wall -Iinclude -c src/$@.cpp -o obj/Release/src/$@.o -std=c++11

Type4Entry: src/Type4Entry.cpp include/Type4Entry.h
	$(CXX) -Wall -Iinclude -c src/$@.cpp -o obj/Release/src/$@.o -std=c++11

Type5Or15Entry: src/Type5Or15Entry.cpp include/Type5Or15Entry.h
	$(CXX) -Wall -Iinclude -c src/$@.cpp -o obj/Release/src/$@.o -std=c++11

Type5Entry: src/Type5Entry.cpp include/Type5Entry.h
	$(CXX) -Wall -Iinclude -c src/$@.cpp -o obj/Release/src/$@.o -std=c++11

Type6Entry: src/Type6Entry.cpp include/Type6Entry.h
	$(CXX) -Wall -Iinclude -c src/$@.cpp -o obj/Release/src/$@.o -std=c++11

Type7Entry: src/Type7Entry.cpp include/Type7Entry.h
	$(CXX) -Wall -Iinclude -c src/$@.cpp -o obj/Release/src/$@.o -std=c++11

Type8Entry: src/Type8Entry.cpp include/Type8Entry.h
	$(CXX) -Wall -Iinclude -c src/$@.cpp -o obj/Release/src/$@.o -std=c++11

Type9Entry: src/Type9Entry.cpp include/Type9Entry.h
	$(CXX) -Wall -Iinclude -c src/$@.cpp -o obj/Release/src/$@.o -std=c++11

Type10Entry: src/Type10Entry.cpp include/Type10Entry.h
	$(CXX) -Wall -Iinclude -c src/$@.cpp -o obj/Release/src/$@.o -std=c++11

Type11Entry: src/Type11Entry.cpp include/Type11Entry.h
	$(CXX) -Wall -Iinclude -c src/$@.cpp -o obj/Release/src/$@.o -std=c++11

Type12Entry: src/Type12Entry.cpp include/Type12Entry.h
	$(CXX) -Wall -Iinclude -c src/$@.cpp -o obj/Release/src/$@.o -std=c++11

Type13Entry: src/Type13Entry.cpp include/Type13Entry.h
	$(CXX) -Wall -Iinclude -c src/$@.cpp -o obj/Release/src/$@.o -std=c++11

Type14Entry: src/Type14Entry.cpp include/Type14Entry.h
	$(CXX) -Wall -Iinclude -c src/$@.cpp -o obj/Release/src/$@.o -std=c++11

Type15Entry: src/Type15Entry.cpp include/Type15Entry.h
	$(CXX) -Wall -Iinclude -c src/$@.cpp -o obj/Release/src/$@.o -std=c++11

CompleteID: src/CompleteID.cpp include/CompleteID.h
	$(CXX) -Wall -Iinclude -c src/$@.cpp -o obj/Release/src/$@.o -std=c++11

CIDsSet: src/CIDsSet.cpp include/CIDsSet.h
	$(CXX) -Wall -Iinclude -c src/$@.cpp -o obj/Release/src/$@.o -std=c++11

ContactInfo: src/ContactInfo.cpp include/ContactInfo.h
	$(CXX) -Wall -Iinclude -c src/$@.cpp -o obj/Release/src/$@.o -std=c++11

BigInt: src/BigInt.cpp include/BigInt.h
	$(CXX) -Wall -Iinclude -c src/$@.cpp -o obj/Release/src/$@.o -std=c++11

RefereeInfo: src/RefereeInfo.cpp include/RefereeInfo.h
	$(CXX) -Wall -Iinclude -c src/$@.cpp -o obj/Release/src/$@.o -std=c++11

NotaryInfo: src/NotaryInfo.cpp include/NotaryInfo.h
	$(CXX) -Wall -Iinclude -c src/$@.cpp -o obj/Release/src/$@.o -std=c++11

libEntriesHandling.a: obj/Release/src/Util.o obj/Release/src/Entry.o obj/Release/src/Type1Entry.o obj/Release/src/Type2Entry.o obj/Release/src/Type3Entry.o obj/Release/src/Type4Entry.o obj/Release/src/Type5Or15Entry.o obj/Release/src/Type5Entry.o obj/Release/src/Type6Entry.o obj/Release/src/Type7Entry.o obj/Release/src/Type8Entry.o obj/Release/src/Type9Entry.o obj/Release/src/Type10Entry.o obj/Release/src/Type11Entry.o obj/Release/src/Type12Entry.o obj/Release/src/Type13Entry.o obj/Release/src/Type14Entry.o obj/Release/src/Type15Entry.o obj/Release/src/CompleteID.o obj/Release/src/CIDsSet.o obj/Release/src/TNtrNr.o obj/Release/src/ContactInfo.o obj/Release/src/BigInt.o obj/Release/src/RefereeInfo.o obj/Release/src/NotaryInfo.o
	ar rcs $@ $^
