
# Behnam Dezfouli
#  CSEN, Santa Clara University

# This program implements a simple smtp mail client to send an email to a local smtp server

# Note:
# Run a local smtp mail server using the following command before running this code:
# python -m smtpd -c DebuggingServer -n localhost:6000


from socket import *
import ssl


# Choose a mail server
mailserver = 'localhost'


# Create socket called clientSocket and establish a TCP connection with mailserver
clientSocket = socket(AF_INET,SOCK_STREAM)


# Port number may change according to the mail server
clientSocket.connect((mailserver,6000))


recv = clientSocket.recv(1024).decode()
print(recv)
if recv[:3] != '220':
    print('220 reply not received from server.')


# Send HELO command along with the server address
# STUDENT WORK



# Send MAIL FROM command and print server response
# STUDENT WORK



# Send RCPT TO command and print server response
# STUDENT WORK



# Send DATA command and print server response
# STUDENT WORK



# Send message data.
# STUDENT WORK



# Message to send
# STUDENT WORK


# Message ends with a single period
# STUDENT WORK


# Send QUIT command and get server response
# STUDENT WORK

