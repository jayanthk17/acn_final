import java.io.*;
import java.net.*;

public class TCPServer {
    public static void main(String[] args) {
        int port = 7891;

        try (ServerSocket serverSocket = new ServerSocket(port)) {
            System.out.println("Server is Online and waiting for connection...");

            while (true) {
                try (Socket clientSocket = serverSocket.accept();
                     BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
                     FileOutputStream fileOutputStream = new FileOutputStream("received_textfile.txt", true);
                     BufferedWriter fileWriter = new BufferedWriter(new OutputStreamWriter(fileOutputStream))) {

                    System.out.println("Client connected.");

                    String line;
                    while ((line = in.readLine()) != null) {
                        fileWriter.write(line);
                        fileWriter.newLine();  // Add a newline character to separate lines
                    }

                    System.out.println("File received from client.");
                } catch (Exception e) {
                    System.out.println("Error handling client: " + e.getMessage());
                }
            }
        } catch (Exception e) {
            System.out.println("Server error: " + e.getMessage());
        }
    }
}

