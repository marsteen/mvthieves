//---------------------------------------------------------------------------
//
// Klasse:    C3DResRef
//
//---------------------------------------------------------------------------

class CG3DResource;
class CG3DResRef
{
    public:

        CG3DResRef()
        {
            mRef = NULL;
        }


        string mName;
        CG3DResource* mRef;
};
