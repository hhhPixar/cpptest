#include "oop-demo.h"

int main() {
    MyDocument document;
    // on_open_file 由基类提供；其中的虚调用仍会分派到 MyDocument::serialize。
    document.on_open_file();
}
