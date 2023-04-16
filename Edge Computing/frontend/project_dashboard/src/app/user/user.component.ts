import { Component, OnInit } from '@angular/core';
import { HttpClient } from '@angular/common/http';



@Component({
  selector: 'app-user',
  templateUrl: './user.component.html',
  styleUrls: ['./user.component.css']
})
export class UserComponent implements OnInit {
  public data: any;
  
  constructor(private http: HttpClient) {
    this.http.get('http://127.0.0.1:5000/logged_user').subscribe(data => {
    this.data = data;  
    console.log(this.data);
  }); 
    
   }

  
  ngOnInit() {  
   
           
  }

}
