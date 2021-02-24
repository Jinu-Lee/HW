public class Library {

	private Book[] book = new Book[10];
	
	Library() {
		for(int i = 0; i < 10; i++)
			book[i] = new Book();
	}
	
	void borrowBook(int n) {
		if(n < 0 || n >= 10) {		// 0 ~ 9
			System.out.println("ERROR : Out of Index (" + n + ")");
		}
		else if(book[n].checkBorrowed()) { 	// if Borrowed
			System.out.println("ERROR : The book #" + n + " is already borrowed.");
		}
		else {
			System.out.println("You can borrow the book #" + n + ".");
			book[n].mark();
		}
	}
	
	void returnBook(int n) {
		if(n < 0 || n >= 10) {		// 0 ~ 9
			System.out.println("ERROR : Out of Index (" + n + ")");
		}
		else if(book[n].checkBorrowed()) { 	// if Borrowed
			System.out.println("You can return the book #" + n + ".");
			book[n].mark();
		}
		else {
			System.out.println("ERROR : The book #" + n + " is already returned.");
		}
	}
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
	}

}
