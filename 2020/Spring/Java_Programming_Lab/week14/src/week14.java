import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

public class week14 {
    public static void main(String[] args) throws Exception {
        BlockingQueue<Integer> q = new ArrayBlockingQueue<>(5);
        
        Consumer c = new Consumer(q);
        Producer p = new Producer(q);

        c.start();
        p.start();
    }
}

class Consumer extends Thread {
    private Thread t;
    BlockingQueue<Integer> q;

    Consumer(BlockingQueue<Integer> q) {
        this.q = q;
    }

    public void run() {
        int i = -1;
        while (i != 9) {
            try {
                i = q.take();
                System.out.println("Consumed: " + i);
                Thread.sleep(1000);
            } catch (Exception e) {

            }
        }
    }

    public void start() {
        if (t == null) {
            t = new Thread(this);
            t.start();
        }
    }
}

class Producer extends Thread {
    private Thread t;
    BlockingQueue<Integer> q;

    Producer(BlockingQueue<Integer> q) {
        this.q = q;
    }

    public void run() {
        int i = 0;
        while (i < 10) {
            try {
                q.put(i);
                System.out.println("Produced: " + i);
                i++;
                Thread.sleep(300);
            } catch (Exception e) {

            }
        }
    }

    public void start() {
        if (t == null) {
            t = new Thread(this);
            t.start();
        }
    }
}