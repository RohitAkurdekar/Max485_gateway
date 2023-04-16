import { Component, OnInit } from '@angular/core';
import { HttpClient } from '@angular/common/http';



@Component({
  selector: 'app-tables',
  templateUrl: './tables.component.html',
  styleUrls: ['./tables.component.css']
})
export class TablesComponent implements OnInit{

  public data1:any;
  public data2:any;
  public tabledata1=[];
  public tabledata2=[];
   

  constructor(private http: HttpClient) { }

  ngOnInit(): void {

    this.http_refresh();

    setInterval(function(){
      this.http_refresh();
       
    }.bind(this), 5000);

};

http_refresh()
{
  this.http.get('http://192.168.77.205:5000/Device2/Data').subscribe(data => {
      this.data2 = data;  
      //  console.log(this.data2);
       this.table2();
      
    });   

  //web api call
  this.http.get('http://192.168.77.205:5000/Device1/Data').subscribe(data => {
      this.data1 =data; 
      // console.log(this.data1)
      this.table1();
      
    });


}

  columns2 = ['Timestamp', 'Value'];

  setHeader2(title, index) {
    return title ;
  }

  setData2(data, title) {
    return data;
  }

  table2()
  {
    this.tabledata2 = []
    for(let mydata of this.data2)
    {
      // console.log(mydata)
      let tempData = {
        "Timestamp": mydata["_id"],
        "Value": mydata["VALUE"]
      }
      this.tabledata2.push(tempData);
    }
    // console.log(this.tabledata2);

  }



  columns1 = ['Timestamp', 'Pressure','Temperature','Altitude'];

  setHeader1(title, index) {
    return title ;
  }

  setData1(data, title) {
    return data;
  }

  table1()
  {
    this.tabledata1 = []
    for(let mydata of this.data1)
    {
      // console.log(mydata)
      let tempData = {
        "Timestamp": mydata["_id"],
        "Pressure": mydata["PRESSURE"],
        "Temperature": mydata["TEMPERATURE"],
        "Altitude": mydata["ALTITUDE"]
      }
      this.tabledata1.push(tempData);
    }
    // console.log(this.tabledata2);

  }

 


    
}


