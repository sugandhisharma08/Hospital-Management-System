// main.cpp - program entry point
// Run from the project folder:   ./hospital            (uses the "data" folder)
//                                ./hospital my_folder  (uses another data folder)
#include <iostream>
#include <string>
#include <exception>
#include "Hospital.h"
#include "Utils.h"

int main(int argc, char* argv[]) {
    std::string dataDir = "data";
    if (argc > 1) dataDir = argv[1];

    try {
        HospitalSystem hospital(dataDir);
        hospital.run();
    } catch (const InputEnded&) {
        std::cout << "\nInput ended. Data was saved after every change. Exiting.\n";
    } catch (const std::exception& e) {
        std::cout << "\nFatal error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
