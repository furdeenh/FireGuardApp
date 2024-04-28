import pexpect
import time

# Constants
HOST = "128.4.222.193"
USER = "furdeenh"
PASSWORD = "fardeen211
"
SSH_COMMAND = f"ssh -Y {USER}@{HOST}"
COMMAND = "sudo -E ircamera I2C MLX90640"

def ssh_login(host, user, password, command):
    try:
        # Start the SSH connection
        session = pexpect.spawn(SSH_COMMAND, timeout=300000)  # Adjusted timeout for flexibility
        session.expect("password:")  # Wait for the password prompt
        
        # Send the password
        session.sendline(password)
        
        # Wait for the shell prompt, indicating we're logged in and ready
        session.expect("\$")  # This might need to be adjusted if the prompt is different
        
        # Send the command
        session.sendline(command)
        
        # Wait to capture some output if needed, adjust timeout as necessary
        session.expect("\$")  # Wait for the command to complete and return to the prompt

        # Optionally print the output
        print("Command output:", session.before.decode('utf-8'))

        # Allow interaction with the terminal
        session.interact()

        # Close the session after interact returns (user exits the remote shell)
        session.close()
    except pexpect.exceptions.EOF:
        print("EOF encountered, possibly connection failure.")
    except pexpect.exceptions.TIMEOUT:
        print("Timeout occurred, check network or server.")
    except Exception as e:
        print(f"An error occurred: {e}")

# Run the function
if __name__ == "__main__":
    ssh_login(HOST, USER, PASSWORD, COMMAND)

