# Docker file for MODBUS project mongo storage

FROM node:alpine
LABEL developer="MODBUS_team"
RUN npm install -g @angular/cli
WORKDIR /usr/app
COPY ./ /usr/app
WORKDIR /usr/app/project_dashboard
RUN npm install
EXPOSE 4200/tcp
# CMD ["ng", "serve", "--host", "0.0.0.0"]