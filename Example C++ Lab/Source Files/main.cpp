//-----------------------------------------------------------------------------
// PROJECT      : MAIN
// FILE         : MAIN.CPP
// FILE VERSION : 2.4
// PROGRAMMER   : JOSIAH LEDUA
//-----------------------------------------------------------------------------
// REVISION HISTORY
//-----------------------------------------------------------------------------
//
// 2.3, 2018-12-01, Josiah Ledua
//   - Working w/ all argument type except void types
//   - Function do not return values (must pass by ref)
//
//-----------------------------------------------------------------------------
// MODULE DESCRIPTION
//-----------------------------------------------------------------------------
//
// The main file is a the unviversal menu system to be use for other projects
//
//-----------------------------------------------------------------------------
// INCLUDE FILES
//-----------------------------------------------------------------------------

#include "global.h"
#include "menu.h"

//-----------------------------------------------------------------------------
// DEFINITIONS
//-----------------------------------------------------------------------------

#define STANDARD_RATE 10
#define TWO_DAY_FLAT_RATE 5
#define OVERNIGHT_RATE 2

//-----------------------------------------------------------------------------
// CLASS    : Address 
//-----------------------------------------------------------------------------

class Address
{
public:
	Address() { return; }
	Address(std::string psFirstName, std::string psLastName, std::string psState, std::string psCity, std::string psNumber, std::string psStreet, std::string psZip):
		FirstName(psFirstName) , LastName(psLastName), State(psState), City(psCity), Number(psNumber), Street(psStreet), Zip(psZip){}
	std::string getAddress(void) { return Number + " " + Street + " " + City + ", " + State + " " + Zip; }
	std::string getName(void) { return FirstName + " " + LastName; }
private:
	std::string FirstName, LastName, City, Number, Street, State, Zip;
};

//-----------------------------------------------------------------------------
// CLASS: Package
//-----------------------------------------------------------------------------

class Package
{
public:
	Package() { return; }
	Package(Address pSendAddr, Address pReciveAddr, double piWeight, double piCostPerOunce)
	{
		Type = "Standard";

		SendAddr = pSendAddr;
		ReciveAddr = pReciveAddr;

		setWeight(piWeight);
		setCostPerWeight(piCostPerOunce);
	}
	std::string getType(void) { return Type; }
	virtual double calculateCost(void) { return Weight * CostPerWeight; }
	void printPackage(void)
	{
		std::cout << "  From  : " << getReciveName() << " - " << getReciveAddress() << std::endl;
		std::cout << "  To    : " << getSenderName() << " - " << getSenderAddress() << std::endl;

		std::cout << "  Weight: " << std::fixed << std::setprecision(0) << getWeight() << " oz." << std::endl;
		std::cout << "  Cost  : $" << std::fixed << std::setprecision(2) << calculateCost() << std::endl << std::endl;
		return;
	}
private:	
	std::string getReciveName(void) { return ReciveAddr.getName(); }
	std::string getReciveAddress(void) { return ReciveAddr.getAddress(); }
	std::string getSenderName(void) { return SendAddr.getName(); }
	std::string getSenderAddress(void) { return SendAddr.getAddress(); }
	double getWeight(void) { return Weight; }
protected:
	void setWeight(double piWeight) { Weight = ((piWeight >= 0) ? piWeight : 0); }
	void setCostPerWeight(double piCostPerWeight) { CostPerWeight = ((piCostPerWeight >= 0) ? piCostPerWeight : 0); }
	Address SendAddr;
	Address ReciveAddr;
	double Weight;
	double CostPerWeight;
	std::string Type;
};

//-----------------------------------------------------------------------------
// CLASS: TwoDayPackage
//-----------------------------------------------------------------------------

class TwoDayPackage : public Package
{
public:
	TwoDayPackage() { return; }
	TwoDayPackage(Address pSendAddr, Address pReciveAddr, double piWeight, double piCostPerOunce, double pdExtraCost)
	{
		Type = "Two-Day";

		SendAddr = pSendAddr;
		ReciveAddr = pReciveAddr;

		setWeight(piWeight);
		setCostPerWeight(piCostPerOunce);
		setExtraCost(pdExtraCost);
	}
	double calculateCost(void) { return (Weight * CostPerWeight) + ExtraCost; }
private:
	int ExtraCost = 0;
	void setExtraCost(double pdExtraCost) { ExtraCost = ((pdExtraCost >= 0) ? pdExtraCost : 0); }
};

//-----------------------------------------------------------------------------
// CLASS: OverNightPackage
//-----------------------------------------------------------------------------

class OverNightPackage : public Package
{
public:
	OverNightPackage() { return; }
	OverNightPackage(Address pSendAddr, Address pReciveAddr, double piWeight, double piCostPerOunce, double pdExtraCost)
	{
		Type = "Over Night";

		SendAddr = pSendAddr;
		ReciveAddr = pReciveAddr;

		setWeight(piWeight);
		setCostPerWeight(piCostPerOunce);
		setExtraCost(pdExtraCost);

	}
	double calculateCost(void) { return (Weight * CostPerWeight * ExtraCost); }
private:
	double ExtraCost = 0;
	void setExtraCost(double pdExtraCost) { ExtraCost = ((pdExtraCost >= 0) ? pdExtraCost : 0); }

};

//-----------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//-----------------------------------------------------------------------------

int newContact(std::vector<Address*> *pContactsPrt, Address* pSelected);
int selContact(std::vector<Address*>, Address *, size_t);

int printOrders(std::vector<Package*>*);
int newShippment(std::vector<Package*>*, std::vector<Address*>*);

int pickContact(std::string pTitle, std::vector<Address*> *pContacts, Address *pSelected);

int newStandard(std::vector<Package*>*, Address, Address, double);
int newTwoDay(std::vector<Package*>*, Address, Address, double);
int newOvernight(std::vector<Package*>*, Address, Address, double);


//-----------------------------------------------------------------------------
// FUNCTION : main()
// PURPOSE  : 
//-----------------------------------------------------------------------------

int main(int)
{
	std::vector<Package*> Orders;
	std::vector<Address*> Contacts;
	Address TempContact("John", "Smith", "FL", "Orlando", "1", "Main St.", "32804");


	Contacts.push_back(new Address("John", "Smith", "FL", "Orlando", "1", "Main St.", "32804"));
	Contacts.push_back(new Address("Todd", "Dawson", "NY", "New York", "1", "Main St.", "10044"));
	Contacts.push_back(new Address("Bruce", "Wayne", "NY", "Gotham", "1007", "Mountain Dr.", "53540"));

	Orders.push_back(new Package(*Contacts[0], *Contacts[1], 10, STANDARD_RATE));
	Orders.push_back(new TwoDayPackage(*Contacts[1], *Contacts[0], 10, STANDARD_RATE, TWO_DAY_FLAT_RATE));
	Orders.push_back(new OverNightPackage(*Contacts[2], *Contacts[0], 10, STANDARD_RATE, OVERNIGHT_RATE));


	Menu mainMenu("Main Menu");
	mainMenu.AddLine(ESC, "Exit");
	mainMenu.AddLine(new Action<int, std::vector<Package*>*>(new KeyLst{'1'}, new TextLst{ "View Shipments" }, printOrders, &Orders));
	mainMenu.AddLine(new Action<int, std::vector<Package*>*, std::vector<Address*>*>(new KeyLst{ '2' }, new TextLst{ "New Shipment" }, newShippment, &Orders, &Contacts));
	mainMenu.AddLine(new Action<int, std::vector<Address*>*, Address *>(new KeyLst{ '3','N','n' }, new TextLst{ "Add Address" }, newContact, &Contacts, &TempContact));

	while (mainMenu.Show());

	CLEAR;
	std::cout << "Exiting Program" << std::endl;
	PAUSE;

	// Delete Orders
	for (size_t i = 0; i < Orders.size(); i++) free(Orders[i]);
	
	// Delete Contacts
	for (size_t i = 0; i < Contacts.size(); i++) free(Contacts[i]);

	return 0;
}

//-----------------------------------------------------------------------------
// FUNCTION : main()
// PURPOSE  : 
//-----------------------------------------------------------------------------

int newContact(std::vector<Address*> * pContactsPrt, Address * pSelected)
{
	std::string psFirstName, psLastName, psCity, psState, psNumber, psStreet, psZip;

	CLEAR;

	std::cout << "Enter First Name: ";
	std::getline(std::cin, psFirstName);

	std::cout << "Enter Last Name: ";
	std::getline(std::cin, psLastName);

	std::cout << "Enter City: ";
	std::getline(std::cin, psCity);

	std::cout << "Enter State/Province: ";
	std::getline(std::cin, psState);

	std::cout << "Enter Street Number: ";
	std::getline(std::cin, psNumber);

	std::cout << "Enter Street Name: ";
	std::getline(std::cin, psStreet);

	std::cout << "Enter ZIP/Postal Code: ";
	std::getline(std::cin, psZip);

	Address *newContact = new Address(psFirstName, psLastName, psState, psCity , psNumber, psStreet, psZip);

	pSelected = newContact;

	pContactsPrt->push_back(newContact);
	
	return 0;
}

//-----------------------------------------------------------------------------
// FUNCTION : newShippment()
// PURPOSE  : 
//-----------------------------------------------------------------------------

int newShippment(std::vector<Package*> *pOrders, std::vector<Address*> *pContacts)
{
	double pWeight = 0.0;
	Address pDestAddress, pSendAddress;

	pickContact("Select destination Addess", pContacts, &pDestAddress);

	pickContact("Select Sender Addess", pContacts, &pSendAddress);

	CLEAR;
	std::cout << "Enter Package weight ";
	std::cin >> pWeight;

	Menu typeMenu("Select Package Type",false);
	typeMenu.AddLine(new Option(new KeyLst{ ESC }, new TextLst{ "Cancel Shippment" }));
	typeMenu.AddLine(new Action<int, std::vector<Package*>*, Address, Address, double>(new KeyLst{ '1' }, new TextLst{ "Standard ($10/oz.)"}, newStandard, pOrders, pSendAddress, pDestAddress, pWeight));
	typeMenu.AddLine(new Action<int, std::vector<Package*>*, Address, Address, double>(new KeyLst{ '2' }, new TextLst{ "Two Day ($10/oz. + $5)" }, newTwoDay, pOrders, pSendAddress, pDestAddress, pWeight));
	typeMenu.AddLine(new Action<int, std::vector<Package*>*, Address, Address, double>(new KeyLst{ '3' }, new TextLst{ "Overnight ($20/oz.)" }, newOvernight, pOrders, pSendAddress, pDestAddress, pWeight));

	while (typeMenu.Show());

	return 0;
} // End newShippment()

//-----------------------------------------------------------------------------
// FUNCTION : pickContact()
// PURPOSE  : 
//-----------------------------------------------------------------------------

int pickContact(std::string pTitle,std::vector<Address*> *pContacts, Address *pSelected)
{
	Menu pickMenu(pTitle,false);
	pickMenu.AddLine(new Option(new KeyLst(1, ESC), new TextLst(1, "Skip(Leave Blake)")));
	for (size_t i = 0; i < pContacts->size(); i++)
	{
		pickMenu.AddLine(new Action<int, std::vector<Address*>, Address *, size_t>(new KeyLst{ (char)('1' + i) }, new TextLst{ (*pContacts)[i]->getName() + " - " + (*pContacts)[i]->getAddress() }, selContact, *pContacts, pSelected, i));
	}
	pickMenu.AddLine(new Action<int, std::vector<Address*>*, Address *>(new KeyLst{ 'N','n' }, new TextLst{ "New Address" }, newContact, pContacts, pSelected));

	while (pickMenu.Show());

	return 0;
} // End pickContact()

//-----------------------------------------------------------------------------
// FUNCTION : printContact()
// PURPOSE  : 
//-----------------------------------------------------------------------------

int selContact(std::vector<Address*> pContacts, Address *pSelected, size_t piIndex)
{
	*pSelected = *pContacts[piIndex];

	return 0;
} // End selContact()


//-----------------------------------------------------------------------------
// FUNCTION : printOrders()
// PURPOSE  : 
//-----------------------------------------------------------------------------

int printOrders(std::vector<Package*> *pOrders)
{
	CLEAR;

	// Print Costs
	for (size_t i = 0; i < pOrders->size(); i++)
	{
		std::cout << "Order " << i << " is a " << (*pOrders)[i]->getType() << " package" << std::endl;
		(*pOrders)[i]->printPackage();
	}

	PAUSE;

	return 0;
} // printOrders()

//-----------------------------------------------------------------------------
// FUNCTION : newStandard()
// PURPOSE  : 
//-----------------------------------------------------------------------------

int newStandard(std::vector<Package*> *pOrders, Address pSendAddr, Address pReciveAddr, double piWeight)
{
	pOrders->push_back(new Package(pSendAddr, pReciveAddr, piWeight, STANDARD_RATE));

	return 0;
} // End newStandard()

//-----------------------------------------------------------------------------
// FUNCTION : newTwoDay()
// PURPOSE  : 
//-----------------------------------------------------------------------------

int newTwoDay(std::vector<Package*> *pOrders, Address pSendAddr, Address pReciveAddr, double piWeight)
{
	pOrders->push_back(new TwoDayPackage(pSendAddr, pReciveAddr, piWeight, STANDARD_RATE,TWO_DAY_FLAT_RATE));

	return 0;
} // End newTwoDay()

//-----------------------------------------------------------------------------
// FUNCTION : newOvernight()
// PURPOSE  : 
//-----------------------------------------------------------------------------

int newOvernight(std::vector<Package*> *pOrders, Address pSendAddr, Address pReciveAddr, double piWeight)
{
	pOrders->push_back(new OverNightPackage(pSendAddr, pReciveAddr, piWeight, STANDARD_RATE, OVERNIGHT_RATE));
	return 0;

} // End newOvernight()

//-----------------------------------------------------------------------------
// END MAIN.CPP
//-----------------------------------------------------------------------------
