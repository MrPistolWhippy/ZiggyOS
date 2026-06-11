struct user_profile { char username[16]; unsigned int password_hash; int active; };
struct user_profile system_users[5];
int total_system_users = 0;
void app_init_users() {
system_users[0].active = 1; system_users[0].password_hash = 208947265;
unsigned char* adm = (unsigned char*)"admin";
for(int i=0; adm[i]!='\0'; i++) system_users[0].username[i] = adm[i];
total_system_users = 1;
}
void app_user_add(const char* name, const char* raw_pass) {
if (total_system_users >= 5) return;
int slot = total_system_users;
system_users[slot].active = 1;
system_users[slot].password_hash = app_calculate_string_hash(raw_pass);
int i = 0; while(name[i] != '\0' && i < 15) { system_users[slot].username[i] = name[i]; i++; }
system_users[slot].username[i] = '\0'; total_system_users++;
print("User profile registered.\n");
}
void app_crypt_file() {
struct file_struct { char n[32]; char c[128]; int s; int a; };
extern struct file_struct ramfs[10];
print("\nEncrypting RAMFS files...\n");
if(ramfs[0].a == 1) {
char key = (char)(target_password_hash & 0xFF);
for(int i = 0; ramfs[0].c[i] != '\0' && i < 128; i++) ramfs[0].c[i] ^= key;
print("Cipher shift sequence complete.\n");
}
}
void app_set_system_theme(char selection) {
extern char current_color;
if(selection == 't') current_color = 0x03;
else if(selection == 'e') current_color = 0x0A;
else if(selection == 'a') current_color = 0x06;
print("Color spectrum flipped.\n");
}
void app_crypt_file() {
struct file_struct { char n[32]; char c[128]; int s; int a; };
extern struct file_struct ramfs[10];
print("\nEncrypting RAMFS files...\n");
if(ramfs[0].a == 1) {
char key = (char)(target_password_hash & 0xFF);
for(int i = 0; ramfs[0].c[i] != '\0' && i < 128; i++) ramfs[0].c[i] ^= key;
print("Cipher shift sequence complete.\n");
}
}
void app_set_system_theme(char selection) {
extern char current_color;
if(selection == 't') current_color = 0x03;
else if(selection == 'e') current_color = 0x0A;
else if(selection == 'a') current_color = 0x06;
print("Color spectrum flipped.\n");
}
