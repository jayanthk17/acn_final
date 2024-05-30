import java.net.*;
import java.io.*;

public class UDPClient {
    public static void main(String[] args) {
        String serverAddress = "127.0.0.1";
        int port = 7891;
        byte[] buffer = new byte[1024];

        try (DatagramSocket socket = new DatagramSocket()) {
            InetAddress address = InetAddress.getByName(serverAddress);

            System.out.print("Enter message: ");
            BufferedReader consoleInput = new BufferedReader(new InputStreamReader(System.in));
            String message = consoleInput.readLine();
            buffer = message.getBytes();

            DatagramPacket packet = new DatagramPacket(buffer, buffer.length, address, port);
            socket.send(packet);
            System.out.println("Message sent to server.");

        } catch (Exception e) {
            System.out.println("Client error: " + e.getMessage());
        }
    }
}
