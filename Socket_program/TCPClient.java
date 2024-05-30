import java.io.*;
import java.net.*;

public class TCPClient {
    public static void main(String[] args) {
        String serverAddress = "127.0.0.1";
        int port = 7891;

        try (Socket socket = new Socket(serverAddress, port);
             BufferedWriter out = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
             BufferedReader fileReader = new BufferedReader(new FileReader("/home/jayanth/Downloads/Socket_prog/Sendtext.txt"))) {
            
            String line;
            while ((line = fileReader.readLine()) != null) {
                out.write(line);
                out.newLine();  // Add a newline character to separate lines
            }
            out.flush(); // Ensure all data is sent

            System.out.println("File sent to server.");

        } catch (Exception e) {
            System.out.println("Client error: " + e.getMessage());
        }
    }
}


