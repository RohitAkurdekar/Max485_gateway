import { Component, OnInit } from '@angular/core';
import Chart from 'chart.js/auto' ;
import { HttpClient } from '@angular/common/http';


@Component({
  selector: 'app-home',
  templateUrl: './home.component.html',
  styleUrls: ['./home.component.css']
})


export class HomeComponent implements OnInit{

  constructor(private http: HttpClient) {}

  public chart: any;
  public chartT: any;
  public chartP: any;
  public chartA: any;

  public data1:any;
  public data2:any;

  public id;
  // public array_value : any ;
  // public array_timestamp : any ;
  

  ngOnInit(): void {


    this.http.get('http://192.168.77.205:5000/Device/Data').subscribe(data => {
           
  });   

  this.http_refresh();

      this.id = setInterval(function(){
        this.http_refresh();        
         
      }.bind(this), 5000);
        

  };
  
  http_refresh()
  {

     this.http.get('http://192.168.77.205:5000/Device2/Data').subscribe(data => {
         this.data2 = data;  
         //  console.log(this.data2);
         this.createChartDev2();
       });   

     //web api call
     this.http.get('http://192.168.77.205:5000/Device1/Data').subscribe(data => {
         this.data1 =data; 
         // console.log(this.data1)
          this.createChartDev1Temp();
          this.createChartDev1Pre();
          this.createChartDev1Alt();
       });

  }
  
  createChartDev2(){   
    var array_values = new Array(30);
    var array_timestamp = new Array(30);
    for (let myindex in this.data2)
    {
      // console.log(myindex);
      array_values[myindex]= this.data2[myindex]["VALUE"].toString();
      array_timestamp[myindex] = this.data2[myindex]["_id"].toString();
    }

    // console.log(array_values);

    if(this.chart) this.chart.destroy();

    this.chart = new Chart("MyChartDev2", {
      type: 'line', //this denotes tha type of chart
      data: {// values on X-Axis
        labels: array_timestamp, 
	       datasets: [
          {
            label: "Value",
            data: array_values,
            borderColor: 'green'
          },
        ]
      },
      options: {
        aspectRatio:2.0
      }
      
    });
  }

  createChartDev1Temp(){    

    var array_values = new Array(30);
    var array_timestamp = new Array(30);

  for (let myindex in this.data1)
  {
    // console.log(myindex);
    array_values[myindex]= this.data1[myindex]["TEMPERATURE"].toString();
    array_timestamp[myindex] = this.data1[myindex]["_id"].toString();
  }
    // console.log(array_values);
    if(this.chartT) this.chartT.destroy();

    this.chartT = new Chart("MyChartDev1Temp", {
      type: 'line', //this denotes tha type of chart
      data: {// values on X-Axis
        labels: array_timestamp, 
	       datasets: [
          {
            label: "Temprature",
            data: array_values,
            borderColor: 'red'
          },
        ]
      },
      options: {
        aspectRatio:2.0
      }
      
    });
  }

  createChartDev1Pre(){    
    var array_values = new Array(30);
    var array_timestamp = new Array(30);

  for (let myindex in this.data1)
  {
    // console.log(myindex);
    array_values[myindex]= this.data1[myindex]["PRESSURE"].toString();
    array_timestamp[myindex] = this.data1[myindex]["_id"].toString();
  }
    // console.log(array_values);

    if(this.chartP) this.chartP.destroy();

    this.chartP = new Chart("MyChartDev1Pre", {
      type: 'line', //this denotes tha type of chart
      data: {// values on X-Axis
        labels: array_timestamp, 
	       datasets: [
          {
            label: "Pressure",
            data: array_values,
            borderColor: 'yellow'
          },
        ]
      },
      options: {
        aspectRatio:2.0
      }
      
    });
  }

  createChartDev1Alt(){    

    var array_values = new Array(30);
    var array_timestamp = new Array(30);

  for (let myindex in this.data1)
  {
    // console.log(myindex);
    array_values[myindex]= this.data1[myindex]["ALTITUDE"].toString();
    array_timestamp[myindex] = this.data1[myindex]["_id"].toString();
  }
    // console.log(array_values);

    if(this.chartA) this.chartA.destroy();

    this.chartA = new Chart("MyChartDev1Alt", {
      type: 'line', //this denotes tha type of chart
      data: {// values on X-Axis
        labels: array_timestamp, 
	       datasets: [
          {
            label: "Altitude",
            data: array_values,
            borderColor: 'blue'
          },
        ]
      },
      options: {
        aspectRatio:2.0
      }
      
    });
  }
  
  
    
  
}

