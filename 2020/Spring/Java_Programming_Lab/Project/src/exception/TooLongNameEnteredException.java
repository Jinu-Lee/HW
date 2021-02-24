package exception;

public class TooLongNameEnteredException extends RuntimeException{

    /**
     *
     */
    private static final long serialVersionUID = 1L;

    public TooLongNameEnteredException() {
		// TODO Auto-generated constructor stub
	}

	public TooLongNameEnteredException(String message) {
		super(message);
		// TODO Auto-generated constructor stub
	}

	public TooLongNameEnteredException(Throwable cause) {
		super(cause);
		// TODO Auto-generated constructor stub
	}

	public TooLongNameEnteredException(String message, Throwable cause) {
		super(message, cause);
		// TODO Auto-generated constructor stub
	}

	public TooLongNameEnteredException(String message, Throwable cause, boolean enableSuppression,
			boolean writableStackTrace) {
		super(message, cause, enableSuppression, writableStackTrace);
		// TODO Auto-generated constructor stub
	}
    
}