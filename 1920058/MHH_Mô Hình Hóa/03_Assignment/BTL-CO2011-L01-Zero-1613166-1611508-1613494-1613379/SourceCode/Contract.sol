pragma solidity ^0.4.17;

import "./DateTime.sol";
contract Owner_Staff{
	address public owner;
	address public staff;
	uint public salary_money;
	uint public deposit_money;
	string strtime;
	uint public timestamp1;
	uint public timestamp2;
	DateTime dt;
	uint public percent_accept;
	bool public cancel_owner;
	bool public cancel_staff;
	uint public time_working_actual;
	uint public time_working_register;

	bool public k;
	
	constructor(address addDateTime) public{
		dt = DateTime(addDateTime);
		owner = msg.sender;
	}

	function set1(uint x) public{
		percent_accept = x;
	}

	function set2(bool x) public{
		cancel_owner = x;
	}

	function set3(bool x) public{
		cancel_staff = x;
	}	

	function set4(bool x) public{
		cancel_staff = x;
	}

	function set5(uint x) public{
		time_working_actual = x;
	}

	function set6(uint x) public{
		time_working_register = x;
	}	

	function sendSalary() public payable{
		if(owner == msg.sender){
			salary_money += msg.value;
		}
	}

	function createStaff() public{
		staff = msg.sender;
	}

	function sendDeposit() public payable{
		if(staff == msg.sender){
			deposit_money += msg.value;
		}
	}

	function createTimestamp1(uint16 year, uint8 month, uint8 day, uint8 hour, uint8 minute) public{
		timestamp1 = dt.toTimestamp(year,month,day,hour,minute);
	}

	function createTimestamp2(uint16 year, uint8 month, uint8 day, uint8 hour, uint8 minute) public{
		timestamp2 = dt.toTimestamp(year,month,day,hour,minute);
	}


	function transfer1(uint16 year, uint8 month, uint8 day, uint8 hour, uint8 minute) public payable{
		if(timestamp1 == dt.toTimestamp(year,month,day,hour,minute)){
			if(salary_money>0 && deposit_money>0){
				k=true;
			}
		}
		else if ((timestamp1 < dt.toTimestamp(year,month,day,hour,minute)) && (deposit_money>0) && (salary_money==0)){
			staff.transfer(deposit_money);
			deposit_money -= deposit_money;
		}
		else if((timestamp1 < dt.toTimestamp(year,month,day,hour,minute)) && (deposit_money==0) && (salary_money>0)){
			owner.transfer(salary_money);
			salary_money -= salary_money;
		}
	}

	function transfer2(uint16 year, uint8 month, uint8 day, uint8 hour, uint8 minute) public payable{
		if((timestamp2 == dt.toTimestamp(year,month,day,hour,minute)) && (k==true)){
			if(time_working_actual == time_working_register){
				staff.transfer(deposit_money);
				deposit_money -= deposit_money;
				staff.transfer(salary_money);
				salary_money -= salary_money;
			}
			else if((cancel_staff == true) && (time_working_actual >= (percent_accept/100)*time_working_register)){
				staff.transfer(salary_money);
				salary_money -= salary_money;
				owner.transfer(deposit_money);
				deposit_money -= deposit_money;
			}
			else if((cancel_owner == true) && (time_working_actual >= (percent_accept/100)*time_working_register)){
				staff.transfer(deposit_money);
				deposit_money -= deposit_money;
				staff.transfer(salary_money);
				salary_money -= salary_money;	
			}
			else if((cancel_staff == true) && (time_working_actual < (percent_accept/100)*time_working_register)){
				owner.transfer(deposit_money);
				deposit_money -= deposit_money;
				owner.transfer(salary_money);
				salary_money -= salary_money;	
			}
			else if((cancel_owner == true) && (time_working_actual < (percent_accept/100)*time_working_register)){
				staff.transfer(deposit_money);
				deposit_money -= deposit_money;
				staff.transfer(salary_money);
				salary_money -= salary_money;	
			}
		}
	}
}
