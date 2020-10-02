//---------------------------------------------------------------------------
//
// PROJECT : Die Planeten
//
//
// AUTOR   : Martin Steen
//           email: martin@martin-steen.de
//
//
//----------------------------------------------------------------------------

#include "CG3DResourceList.h"

int CG3DResourceImgRef::UnscaledWidth()
{
    return (mImgRef != NULL) ? mImgRef->Width(mStdImage) : -1;
}


int CG3DResourceImgRef::UnscaledHeight()
{
    return (mImgRef != NULL) ? mImgRef->Height(mStdImage) : -1;
}
