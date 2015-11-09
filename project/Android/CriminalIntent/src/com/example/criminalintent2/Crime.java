package com.example.criminalintent2;

import java.util.UUID;

public class Crime {
	
	private UUID mId;
	private String mTitle;
	
	public Crime() {
		mId = UUID.randomUUID();
	}

	public String getTitle() {
		return mTitle;
	}

	public void setTitle(String title) {
		mTitle = title;
	}

	public UUID getId() {
		return mId;
	}
	
}
