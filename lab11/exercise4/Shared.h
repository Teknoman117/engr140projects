#ifndef LA9_Shared_h
#define LA9_Shared_h

class Shared {
    private :
    unsigned int _ref;
    
    protected :
    /*! Constructor initializes the reference counter as 0 */
    Shared () {
		_ref=0;
    }
    
    /*! Destructor in derived classes should be declared as
     protected in order to force users to call always unref()
     instead of delete */
    virtual ~Shared () {
    
    }
    
    public :
    
    /*! Returns the current reference counter value. */
    unsigned getref () const {
        return _ref;
    }
    
    /*! Increments the reference counter. */
    void ref () {
        _ref++;
    }
    
    /*! Decrements the reference counter (if >0), and if the
     counter becomes 0, the object is automatically self deleted. */
    void unref() {
        if(_ref>0){
            _ref--;
        }
        if(_ref==0){
            delete this;  
        }
    }
};

#endif
