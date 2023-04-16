#!/usr/bin/python
from flask import Flask, jsonify, abort, make_response, request,render_template,redirect,session
from flask_cors import CORS
import subscriber
import get
import logging
import user




logging.basicConfig(filename='api.log', level=logging.ERROR)

# User Configurable settings

app = Flask(__name__, template_folder='templates')

CORS(app)

#mqtt subscribe and  Storing the values in our
@app.route('/Device/Data',methods=['GET'])
def create_reading():

    logging.info('Started create_reading()')
    subscriber.mqtt_sub()
    

@app.route('/Device1/Data', methods=['GET'])
def get_Dev1():
    data = make_response(jsonify(get.get_last_30("Device1")), 200)
    #print(data)
    return data

    
@app.route('/Device2/Data', methods=['GET'])
def get_Dev2():
     data = make_response(jsonify(get.get_last_30("Device2")), 200)
     #print(data)
     return data

## 404 handler
@app.errorhandler(404)
def page_not_found(e):
  return render_template('404.html'), 404


#internal_server_error
@app.errorhandler(500)
def internal_server_error(e):
    return render_template('500.html'), 500


#  user login
@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        # Get the submitted login form data
        Email = request.form['Email']
        password = request.form['password']
        # Hash the password
       
        print(Email)
        print(password)
        # Check if the username and hashed password exist in the user collection
        users = user.get_user(Email,password)
        if users:
            # Set the user in the session
            print("Login Sucssefull")
            user.details(Email,password)
            session['Email']=Email
            return redirect("http://192.168.77.205:4200/#/dashboard",code=302)
        else:
            print("Login Failed")            
            return render_template('login_failed.html')
            
    else:
        return render_template('login.html')
    
#user details
@app.route('/logged_user',methods=['GET'])
def get_user():
     data = make_response(jsonify(user.logged_user()), 200)
     user.logged_delete()
     return data

#logout
@app.route('/logout')
def logout():
    session.pop('Email',None)
    return redirect('/login')















if __name__ == '__main__':
    app.run(host='0.0.0.0',debug=True)