#include "../header/Encrypted.h"
#include "../header/Credit.h"
#include "../header/methods.h"

int main() {
    Date date1(12, 30);
    Credit cred1("1234 1234 1234 1234", 789, date1);
    cred1.save("info.txt");

    Encrypted enc1(get_text("info.txt"), false);
    //enc1.set_cipher(get_cipher("cipher.txt"));
    enc1.encrypt();
    enc1.print_output("encrypted.txt");
    enc1.decrypt();
    enc1.print_output("output.txt");
    enc1.print_output_key("cipher.txt");
    
    return 0;
}
