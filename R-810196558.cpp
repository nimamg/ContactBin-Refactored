#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <cassert>

using namespace std;

#define COMMA ','
#define AT_SIGN "@"
#define EMPTY ""
#define DOT "."
#define DOT_CHAR '.'
#define AT_SIGN_CHAR '@'
#define PHONE_NUM_LENGTH 11
#define VALID_PHONE_NUM_PREFIX "09"
#define ADD "add"
#define DELETE "delete"
#define SEARCH "search"
#define UPDATE "update"
#define EMPTY_CONTACT initialize_empty_contact();
#define NOTFOUND -1
#define FILENAME "contacts.csv"
#define WHITESPACE ' '

const string failed = "Command Failed.";
const string OK = "Command Ok.";

struct contact { 
	string email;
	string phone;
	string first_name;
	string last_name;
	string address;
	int id;
};

vector <contact> addcontact (vector <contact> contact_list);
vector <contact> deletecontact (vector <contact> contact_list, int id_num);
void search (vector <contact> contact_list);
void printContact (contact Contact);
vector <contact> update (vector <contact> contact_list, int id_num);
contact create_contact_from_file_info (string contact_info);
vector <contact> import_contacts_from_file ();
int create_id_num (vector <contact> &contact_list);
void export_contacts_to_file (vector <contact> contact_list);
bool is_email_valid (string email);
bool is_contact_valid (contact Contact, vector <contact> contact_list);
bool is_contact_valid (contact Contact, vector <contact> contact_list, int index);
bool is_phone_valid (string phone_num);
contact initialize_empty_contact ();
contact create_new_contact (string contact_info);
contact update_contact (contact updated_contact, string update_info);

void test_email_validation ();
void test_phone_validation ();
void test_create_new_contact ();
void test_update_contact ();
void test_initialize_empty_contact ();
void test_contact_validation ();
void test_create_id_num ();
void run_all_tests ();

int main () {
	run_all_tests();

	vector <contact> contact_list;
	contact_list = import_contacts_from_file ();

	string command;
	while (cin >> command) {
	
		if (command == ADD)
			contact_list = addcontact (contact_list) ;

		else if (command == DELETE) {
			int id_to_be_deleted;
			cin >> id_to_be_deleted;
			contact_list = deletecontact (contact_list, id_to_be_deleted);
		}

		else if (command == SEARCH)
			search(contact_list);

		else if (command == UPDATE) {
			int id_to_be_updated;
			cin >> id_to_be_updated;
			contact_list = update(contact_list, id_to_be_updated);
		}

		else
			cout << failed << endl;
	}
	
	export_contacts_to_file (contact_list);

	return 0;
}

vector <contact> addcontact (vector <contact> contact_list) {
	string contact_info;
	getline (cin, contact_info);
	contact new_contact = create_new_contact (contact_info);
	if (!is_contact_valid(new_contact,contact_list)) {
		cout << failed << endl;
		return contact_list;
	}
	new_contact.id = create_id_num(contact_list);
	contact_list.push_back (new_contact);
	cout << OK << endl;
	return contact_list;
}

vector <contact> deletecontact (vector <contact> contact_list, int id_num) {
	for (int i = 0 ; i < contact_list.size() ; i++) {
		if (contact_list[i].id == id_num) {
			contact_list.erase(contact_list.begin() + i);
			cout << OK << endl;
			return contact_list;
		}
	}
	cout << failed << endl;
	return contact_list;
}

void search (vector <contact> contact_list) {
	string text;
	getline(cin,text);
	if (text.size() != 0)
		text = text.substr(1);
	for (int i = 0 ; i < contact_list.size() ; i++) {
		if (contact_list[i].first_name.find(text) != NOTFOUND ||  contact_list[i].last_name.find(text) != NOTFOUND 
			|| contact_list[i].phone.find(text) != NOTFOUND || contact_list[i].email.find(text) != NOTFOUND || contact_list[i].address.find(text) != NOTFOUND)
			printContact (contact_list[i]);
	}
}

void printContact (contact Contact) {
	cout << Contact.id << " " << Contact.first_name << " " << Contact.last_name << " " << Contact.email << " " << Contact.phone << " " << Contact.address << endl;
}

vector <contact> update (vector <contact> contact_list, int id_num) {
	string update_info;
	getline (cin, update_info);
	contact updated_contact = initialize_empty_contact();
	int index;
	for (int i = 0 ; i < contact_list.size() ; i++) {
		if (contact_list[i].id == id_num) {
			updated_contact = contact_list[i];
			index = i;
			break;
		}
	}
	if (!is_contact_valid(updated_contact,contact_list,index)) {
		cout << failed << endl;
		return contact_list;
	}
	updated_contact = update_contact (updated_contact, update_info);
	if (!is_contact_valid(updated_contact,contact_list, index)) {
		cout << failed << endl;
		return contact_list;
	}
	contact_list[index] = updated_contact;
	cout << OK << endl;
	return contact_list;
}

contact create_contact_from_file_info (string contact_info) {
	contact Contact;
	Contact.id = atoi(contact_info.substr(0,contact_info.find(COMMA)).c_str());
	contact_info = contact_info.substr(contact_info.find(COMMA) + 1);
	Contact.first_name = contact_info.substr(0,contact_info.find(COMMA));
	contact_info = contact_info.substr(contact_info.find(COMMA) + 1);
	Contact.last_name = contact_info.substr(0,contact_info.find(COMMA));
	contact_info = contact_info.substr(contact_info.find(COMMA) + 1);
	Contact.email = contact_info.substr(0,contact_info.find(COMMA));
	contact_info = contact_info.substr(contact_info.find(COMMA) + 1);
	Contact.phone = contact_info.substr(0,contact_info.find(COMMA));
	contact_info = contact_info.substr(11);
	if (contact_info.size() != 0)
		Contact.address = contact_info.substr(1);
	return Contact;
}

vector <contact> import_contacts_from_file () {
	vector <contact> contact_list;
	fstream file;
	file.open(FILENAME);
	string contact_info;
	while (getline(file,contact_info)) {
		contact_list.push_back(create_contact_from_file_info(contact_info));
	}
	file.close();
	return contact_list;
}

int create_id_num (vector <contact> &contact_list) {
	if (contact_list.size() == 0)
		return 0;

	return contact_list[contact_list.size()NOTFOUND].id + 1;
}

void export_contacts_to_file (vector <contact> contact_list) { 
	ofstream file;
	file.open(FILENAME, fstream::trunc | fstream::out);
	for (int i = 0 ; i < contact_list.size() ; i++) {
		file << contact_list[i].id << COMMA << contact_list[i].first_name << COMMA << contact_list[i].last_name << COMMA << contact_list[i].email << COMMA << contact_list[i].phone;
		if (contact_list[i].address != EMPTY)
			file << COMMA << contact_list[i].address << endl;
		else
			file << endl;
	}
	file.close();
}

bool is_email_valid (string email) {
	int AT_SIGN_position = email.find_first_of(AT_SIGN);
			if (AT_SIGN_position == NOTFOUND)
				return false;
			if (email.find_first_of(DOT,AT_SIGN_position+1) == NOTFOUND || email.find_first_of(AT_SIGN,AT_SIGN_position+1) != NOTFOUND || email.find_first_of(DOT,AT_SIGN_position+1) == AT_SIGN_position+1 || email.find_first_of(DOT,AT_SIGN_position+1) == email.size()NOTFOUND)
				return false;
			for (int i = 0; i < email.size(); ++i)
				if (isdigit(email[i]) || isalpha(email[i]) || email[i] == AT_SIGN_CHAR || email[i] == DOT_CHAR)					
					continue;
				else
					return false;
			return true;
}

bool is_phone_valid (string phone_num) {
	if (phone_num.size() != PHONE_NUM_LENGTH || phone_num.substr(0,2) != VALID_PHONE_NUM_PREFIX)
				return false;
	for (int i = 0 ; i < phone_num.size() ; i++)
			if (!isdigit(phone_num[i]))
				return false;
	return true;
}
 
contact initialize_empty_contact () {
	contact new_contact;
	new_contact.phone = EMPTY;
	new_contact.email = EMPTY;
	new_contact.first_name = EMPTY;
	new_contact.last_name = EMPTY;
	new_contact.address = EMPTY;
	return new_contact;
}

bool is_contact_valid (contact Contact, vector <contact> contact_list) {
	if (Contact.first_name == EMPTY || Contact.last_name == EMPTY || Contact.email == EMPTY || Contact.phone == EMPTY)
		return false;
	for (int i = 0 ; i < contact_list.size(); i++)
		if ((Contact.first_name == contact_list[i].first_name && Contact.last_name == contact_list[i].last_name) || Contact.phone == contact_list[i].phone || Contact.email == contact_list[i].email)// || Contact.address == contact_list[i].address)
			return false;
	return true;
}

bool is_contact_valid (contact Contact, vector <contact> contact_list, int index) {
	if (Contact.first_name == EMPTY || Contact.last_name == EMPTY || Contact.email == EMPTY || Contact.phone == EMPTY)
		return false;
	if (!is_phone_valid(Contact.phone))
		return false;
	if (!is_email_valid(Contact.email))
		return false;
	for (int i = 0 ; i < contact_list.size(); i++) {
		if (i == index)
			continue;
		if ((Contact.first_name == contact_list[i].first_name && Contact.last_name == contact_list[i].last_name) || Contact.phone == contact_list[i].phone || Contact.email == contact_list[i].email)// || Contact.address == contact_list[i].address)
			return false;
	}
	return true;
}

contact create_new_contact (string contact_info) {
	contact new_contact = initialize_empty_contact();
	stringstream contact_info_stream (contact_info);
	string prefix;
	while (contact_info_stream >> prefix) {
		if (prefix == "-f") {
			contact_info_stream >> new_contact.first_name;
		}
		else if (prefix == "-l") {
			contact_info_stream >> new_contact.last_name;
		}
		else if (prefix == "-e") {
			contact_info_stream >> new_contact.email;
		}
		else if (prefix == "-p") {
			contact_info_stream >> new_contact.phone;
		}
		else if (prefix == "-a") {
			while (contact_info_stream.peek() != EOF && contact_info_stream.peek() != '-') {
				char temp;
				contact_info_stream.get(temp);
				new_contact.address = new_contact.address + temp;
			}
			new_contact.address = new_contact.address.substr(1);
			if (new_contact.address[new_contact.address.size() - 1 == WHITESPACE])
				new_contact.address = new_contact.address.substr(0,new_contact.address.size() - 1);
		}
		else {
			return new_contact;
		}
	}
	return new_contact;
}

contact update_contact (contact updated_contact, string update_info) {
	stringstream update_info_stream (update_info);
	string prefix;
	while (update_info_stream >> prefix) {
		if (prefix == "-f")
			update_info_stream >> updated_contact.first_name;
		else if (prefix == "-l")
			update_info_stream >> updated_contact.last_name;
		else if (prefix == "-e")
			update_info_stream >> updated_contact.email;
		else if (prefix == "-p")
			update_info_stream >> updated_contact.phone;
		else if (prefix == "-a") {
				updated_contact.address = "";
				while (update_info_stream.peek() != EOF && update_info_stream.peek() != '-') {
					char temp;
					update_info_stream.get(temp);
					updated_contact.address = updated_contact.address + temp;
				}
				updated_contact.address = updated_contact.address.substr(1);
				if (updated_contact.address[updated_contact.address.size() - 1 == WHITESPACE])
					updated_contact.address = updated_contact.address.substr(0,updated_contact.address.size() - 1);
		}
		else {
			cout << failed << endl;
			return EMPTY_CONTACT;
		}
	}
	return updated_contact;
}

void test_email_validation () {
	assert(is_email_valid("nima.mg@gmail.com") == true);
	assert(is_email_valid("nima.mg@.com") == false);
	assert(is_email_valid("nima.mg@gmail") == false);
	assert(is_email_valid("nima.mg@gma,il.com") == false);
	assert(is_email_valid("nima.mg@gmail@yahoo.com") == false);
	cout << "Email validation is ok" << endl;
}

void test_phone_validation () {
	assert(is_phone_valid("09182219989") == true);
	assert(is_phone_valid("9182219989") == false);
	assert(is_phone_valid("091822199u9") == false);
	assert(is_phone_valid("091822199893") == false);
	cout << "Phone validation is OK" << endl;
}
void test_create_new_contact () {
	string info = "-f nima -l mg -e nima.mg@gmail.com        -p 09182219989";
	string info_2 = "-f amir -p 09121234567 -l hbv -a tajrish vanak -e amir.hbv@gmail.com";
	contact firstcontact = create_new_contact(info);
	contact secondcontact = create_new_contact(info_2);
	assert (firstcontact.first_name == "nima");
	assert (firstcontact.last_name == "mg");
	assert (firstcontact.email == "nima.mg@gmail.com");
	assert (firstcontact.phone == "09182219989");
	assert (firstcontact.address == "");
	assert (secondcontact.first_name == "amir");
	assert (secondcontact.last_name == "hbv");
	assert (secondcontact.phone == "09121234567");
	assert (secondcontact.email == "amir.hbv@gmail.com");
	assert (secondcontact.address == "tajrish vanak");
	cout << "Create new contact function is ok" << endl;
}

void test_update_contact () {
	contact temp;
	string info = "-f nima -l mg -e nima.mg@gmail.com        -p 09182219989 -a mardookh st";
	temp = create_new_contact(info);
	string command = "-f amir -a tajrish -p 09129876543";
	temp = update_contact(temp,command);
	assert(temp.first_name == "amir");
	assert(temp.last_name == "mg");
	assert(temp.email == "nima.mg@gmail.com");
	assert(temp.phone == "09129876543");
	assert(temp.address == "tajrish");
	cout << "Update contact function is ok" << endl;
}

void test_initialize_empty_contact () {
	contact empty = initialize_empty_contact();
	assert(empty.first_name == EMPTY);
	assert(empty.last_name == EMPTY);
	assert(empty.email == EMPTY);
	assert(empty.phone == EMPTY);
	assert(empty.address == EMPTY);
	cout << "Empty contact initialization is ok" << endl;
}

void test_create_id_num () {
	vector <contact> tempList;
	assert(create_id_num(tempList) == 0);
	string info = "-f nima -l mg -e nima.mg@gmail.com        -p 09182219989 -a mardookh st";
	string info_2 = "-f amir -p 09121234567 -l hbv -a tajrish vanak -e amir.hbv@gmail.com";
	tempList.push_back(create_new_contact(info));
	tempList.push_back(create_new_contact(info_2));
	tempList[0].id = 0;
	tempList[1].id = 1;
	assert(create_id_num(tempList) == 2);
	cout << "Creating id num OK" << endl;
}

void test_contact_validation () {
	vector <contact> list;
	string info = "-f nima -l mg -e nima.mg@gmail.com        -p 09182219989 -a mardookh st";
	string info_2 = "-f amir -p 09121234567 -l hbv -a tajrish vanak -e amir.hbv@gmail.com";
	list.push_back(create_new_contact(info));
	list.push_back(create_new_contact(info_2));
	list[0].id = 0; list[1].id = 1;
	string repeated_name = "-f amir -l hbv -e amir@gmail.com -p 09136567767";
	string correct_contact = "-f nima -l hs -e nhs@gmail.com -p 09188765676";
	string repeated_email = "-f danial -l heiratsajadi -e nima.mg@gmail.com -p 09157786656";
	string repeated_phone = "-f ali -l keramati -p 09121234567 -e alik@gmail.com -a keshavarz blvd";
	contact contact1 = create_new_contact(repeated_name);
	contact contact2 = create_new_contact(repeated_phone);
	contact contact3 = create_new_contact(repeated_email);
	contact contact4 = create_new_contact(correct_contact);
	assert(is_contact_valid(contact1,list) == false);
	assert(is_contact_valid(contact2,list) == false);
	assert(is_contact_valid(contact3,list) == false);
	assert(is_contact_valid(contact4,list) == true);
	cout << "Contact validation OK" << endl;
}

void run_all_tests () {
	test_email_validation();
	test_phone_validation();
	test_initialize_empty_contact();
	test_create_new_contact();
	test_update_contact();
	test_contact_validation();
	test_create_id_num();
	cout << "All tests were OK" << endl;
}