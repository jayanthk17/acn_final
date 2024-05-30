import java.net.*;

public class UDPServer {
    public static void main(String[] args) {
        int port = 7891;
        byte[] buffer = new byte[1024];

        try (DatagramSocket socket = new DatagramSocket(port)) {
            System.out.println("Server is Online");

            DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
            socket.receive(packet);

            String message = new String(packet.getData(), 0, packet.getLength());
            System.out.println("Client: " + message);

        } catch (Exception e) {
            System.out.println("Server error: " + e.getMessage());
        }
    }
}
