import java.io.DataInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.Socket;

class Receiver extends Thread {
	public void run() {
		try {
			Socket soc = new Socket("localhost", 5000);
			DataInputStream dis = new DataInputStream(soc.getInputStream());
			
			int id = dis.readInt();
			
			int dataSize = dis.readInt();
			byte[] buffer = new byte[dataSize];
			
			int totalRecvSize = 0;
			while (totalRecvSize < dataSize) {
				int recvSize = dis.read(buffer, totalRecvSize, dataSize - totalRecvSize);
				totalRecvSize += recvSize;
			}
			
			String file_name = "test-" + id + ".mp4";
			FileOutputStream fos = new FileOutputStream(file_name);
			fos.write(buffer);
			
			System.out.println(dis.readUTF());
			
			dis.close();
			soc.close();
			fos.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}

public class Clients {
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		for (int i = 0; i < 100; i++) {
			new Receiver().start();
		}
	}
}
