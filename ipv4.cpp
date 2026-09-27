#include <iostream>
#include <string>
#include <cctype>

using namespace std;

bool parseNumber(const string& s, int start, int end, int maxValue, int& value) {
    if (start > end) {
        return false;
    }

    int len = end - start + 1;

    if (len > 1 && s[start] == '0') {
        return false;
    }

    value = 0;
    for (int i = start; i <= end; i++) {
        if (!isdigit(static_cast<unsigned char>(s[i]))) {
            return false;
        }
        value = value * 10 + (s[i] - '0');
    }

    return value <= maxValue;
}

bool validateToken(const string& token, unsigned long& outAddress, int& outPort) {
    outAddress = 0;
    outPort = -1;

    int colonPos = -1;

    for (int i = 0; i < static_cast<int>(token.size()); i++) {
        if (token[i] == ':') {
            if (colonPos != -1) {
                return false; // second colon
            }
            colonPos = i;
        }
    }

    string ipPart;
    string portPart;

    if (colonPos == -1) {
        ipPart = token;
    } else {
        ipPart = token.substr(0, colonPos);
        portPart = token.substr(colonPos + 1);

        if (portPart.empty()) {
            return false;
        }

        if (portPart.size() > 5) {
            return false;
        }

        int portValue;
        if (!parseNumber(portPart, 0,
                         static_cast<int>(portPart.size()) - 1,
                         65535, portValue)) {
            return false;
        }

        outPort = portValue;
    }

    int dotCount = 0;
    for (char ch : ipPart) {
        if (ch == '.') {
            dotCount++;
        }
    }

    if (dotCount != 3) {
        return false;
    }

    int octets[4];
    int octetIndex = 0;
    int start = 0;

    for (int i = 0; i <= static_cast<int>(ipPart.size()); i++) {
        if (i == static_cast<int>(ipPart.size()) || ipPart[i] == '.') {
            if (octetIndex >= 4) {
                return false;
            }

            int end = i - 1;

            int len = end - start + 1;
            if (len < 1 || len > 3) {
                return false;
            }

            int value;
            if (!parseNumber(ipPart, start, end, 255, value)) {
                return false;
            }

            octets[octetIndex++] = value;
            start = i + 1;
        }
    }

    if (octetIndex != 4) {
        return false;
    }

    outAddress =
        static_cast<unsigned long>(octets[0]) * 16777216UL +
        static_cast<unsigned long>(octets[1]) * 65536UL +
        static_cast<unsigned long>(octets[2]) * 256UL +
        static_cast<unsigned long>(octets[3]);

    return true;
}

bool extractIPv4(const std::string& str, unsigned long& outAddress, int& outPort) {
    outAddress = 0;
    outPort = -1;

    int n = static_cast<int>(str.size());
    int i = 0;

    while (i < n) {
        bool tokenChar =
            isdigit(static_cast<unsigned char>(str[i])) ||
            str[i] == '.' ||
            str[i] == ':';

        if (!tokenChar) {
            i++;
            continue;
        }

        int start = i;

        while (i < n) {
            bool validTokenChar =
                isdigit(static_cast<unsigned char>(str[i])) ||
                str[i] == '.' ||
                str[i] == ':';

            if (!validTokenChar) {
                break;
            }

            i++;
        }

        string token = str.substr(start, i - start);

        unsigned long addr;
        int port;

        if (validateToken(token, addr, port)) {
            outAddress = addr;
            outPort = port;
            return true;
        }
    }

    return false;
}

int main() {
    string input;

    while (true) {
        cout << "Enter a string (or 'END' to quit): ";
        getline(cin, input);

        if (input == "END") {
            cout << "Program terminated." << endl;
            break;
        }

        unsigned long address;
        int port;

        if (extractIPv4(input, address, port)) {
            unsigned int a = (address >> 24) & 0xFF;
            unsigned int b = (address >> 16) & 0xFF;
            unsigned int c = (address >> 8) & 0xFF;
            unsigned int d = address & 0xFF;

            cout << "Extracted IPv4 address: "
                 << a << "." << b << "." << c << "." << d
                 << " (decimal value: " << address
                 << ", port: ";

            if (port == -1) {
                cout << "none";
            } else {
                cout << port;
            }

            cout << ")" << endl;
        } else {
            cout << "Invalid input: no valid IPv4 address found" << endl;
        }
    }

    return 0;
}