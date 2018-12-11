#include <iostream>

void do_opensubtitles_cpprestsdk();
void opensubtitles_xmlrpc();
//void do_opensubtitles_pistache();
void opensubtitles_cpprestsdk_xmlprc();

namespace subdl { namespace provider { namespace  opensubtitles {
void do_opensubtitles_boost_beast();
}}}


void main_test_build();

int main(int, char *argv[]) {
//    main_test_build();
//    do_opensubtitles_cpprestsdk();
    subdl::provider::opensubtitles::do_opensubtitles_boost_beast();
    //opensubtitles_xmlrpc();
    //opensubtitles_cpprestsdk_xmlprc();
    //do_opensubtitles_pistache();
    return 0;
}
