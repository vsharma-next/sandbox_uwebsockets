FROM ubuntu:latest

# Install necessary package
RUN apt-get update && apt-get install -y \
    g++ \
    make \
    git \
    wget \
    zlib1g-dev

# Set the working directory for the container
WORKDIR /app

# Copy your local application source code and install script into the docker image
COPY ./app.cpp /app/app.cpp
COPY ./install.sh /app/install.sh

# Ensure the script is installable
RUN chmod +x /app/install.sh

# Run your install script
RUN /app/install.sh

# Expose the port your app will run on
EXPOSE 9001

# Command to run your application
# CMD [ "./app.exe" ]
ENTRYPOINT g++ -std=c++20 -I/app/uWebSockets/src -I/app/uWebSockets/uSockets/src /app/simdjson/simdjson.cpp /app/app.cpp /app/uWebSockets/uSockets/*.o -lz -o /app/app.exe && ./app.exe