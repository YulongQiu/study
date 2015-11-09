package com.example.criminalintent;

import java.util.ArrayList;
import java.util.UUID;

import android.content.Context;

/***
 * 
 * @author Lony
 * 
 * Singleton for ArrayList<Crime>, hold data when the App running.
 */

public class CrimeLab {
	
	private static CrimeLab sCrimeLab;
	
	private Context mAppContext;
	private ArrayList<Crime> mCrimes; 
	
	private CrimeLab(Context appContext) {
		mAppContext = appContext;
		mCrimes = new ArrayList<Crime>();
		for (int i = 0; i < 100; i++) {
			Crime crime = new Crime();
			crime.setTitle("Crime #" + i);
			crime.setSolved(i % 2 == 0);
			mCrimes.add(crime);
		}
	}
	
	public static CrimeLab getInstance(Context context) {
		if (sCrimeLab == null) {
			sCrimeLab = new CrimeLab(context.getApplicationContext()); // App Singleton use AppContext
		}
		return sCrimeLab;
	}

	public ArrayList<Crime> getCrimes() {
		return mCrimes;
	}
	
	public Crime getCrime(UUID id) {
		for (Crime crime : mCrimes) {
			if (crime.getId().equals(id)) {
				return crime;
			}
		}
		return null;
	}
}
