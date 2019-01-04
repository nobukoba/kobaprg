#include <iostream>

//     ****************************************************************
double faclog(const int n){
  double faclog_val=0.;
  double fn =1.;
  for (int i=3; i<=n; i++) {
    fn=fn+1.;
    faclog_val=faclog_val+log(fn);
  }
  return faclog_val;
}


//     ****************************************************************
double cleb(const double ria, const double rid, const double rib,
	    const double rie, const double ric, const double rif){
  /* Function of Clebsch-Gordan coefficients
     <j1,m1,j2,m2|JM> = <ria,rid,rib,rie|ric,rif> */
  /* implicit real*8(a-h,o-z)
     common/clebma/faclog(500) */
  int ia=2.*(ria+.0001);
  int ib=2.*(rib+.0001);
  int ic=2.*(ric+.0001);
  int id=2.*(fabs(rid)+.0001);
  int ie=2.*(fabs(rie)+.0001);
  int iF=2.*(fabs(rif)+.0001);
  if (rid<0) {id = -id;}
  if (rie<0) {ie = -ie;}
  if (rif<0) {iF = -iF;}
  double wwww=-1.0;
  double cleb_ret=0.0;
  if((id+ie-iF)!=0) {return cleb_ret;}
  int k1=ia+ib+ic;
  if(pow(-1,k1)<0) {return cleb_ret;}
  if((id==0)&&(ie==0)) {
    k1=k1/2;
    if(pow(-1,k1)<0) {return cleb_ret;}
  }
  k1=ia+ib-ic;
  int k2=ic-abs(ia-ib);
  /*k3=min0(k1,k2);*/
  int k3;
  if (k1<k2) {k3=k1;}
  else       {k3=k2;}
  if(k3<0) {return cleb_ret;}
  if(pow(-1,(ib+ie))<=0) {return cleb_ret;}
  if(pow(-1,(ic+iF))<=0) {return cleb_ret;}
  if((ia-abs (id))<0) {return cleb_ret;}
  if((ib-abs (ie))<0) {return cleb_ret;}
  if((ic-abs (iF))<0) {return cleb_ret;}
  if(ia<0) {//7000,175,165
    return cleb_ret;
  } else if (ia>0) {
    if(ib<0) { //7000,175,170
      return cleb_ret;
    } else if (ib>0){
      if(ic<0) { //7000,180,250
	return cleb_ret;
      } else if (ic==0) {
	double fb=(double)(ib+1);
	cleb_ret=(pow(wwww,(ia-id)/2))/sqrt(fb);
	return cleb_ret;
      }
    }else{
      cleb_ret=1.0;
      return cleb_ret;
    }
  }else{
    cleb_ret=1.0;
    return cleb_ret;
  }
  double fc2=ic+1;
  int iabcp=(ia+ib+ic)/2+1;
  int iabc=iabcp-ic;
  int icab=iabcp-ib;
  int ibca=iabcp-ia;
  int iapd=(ia+id)/2+1;
  int iamd=iapd-id;
  int ibpe=(ib+ie)/2+1;
  int ibme=ibpe-ie;
  int icpf=(ic+iF)/2+1;
  int icmf=icpf-iF;
  double vvv=0.5;
  double sqfclg=vvv*(log(fc2)-faclog(iabcp+1)
		     +faclog(iabc)+faclog(icab)+faclog(ibca)
		     +faclog(iapd)+faclog(iamd)+faclog(ibpe)
		     +faclog(ibme)+faclog(icpf)+faclog(icmf));
  int nzmic2=(ib-ic-id)/2;
  int nzmic3=(ia-ic+ie)/2;
  /* nzmi= max0(0,nzmic2,nzmic3)+1;
     nzmx= min0(iabc,iamd,ibpe);   */
  int nzmi, nzmx;
  if (0>nzmic2) {
    if (0>nzmic3)      { nzmi=0+1;      }
    else               { nzmi=nzmic3+1; }
  }else{
    if (nzmic2>nzmic3) { nzmi=nzmic2+1; }
    else               { nzmi=nzmic3+1; }
  }
  if (iabc<iamd) {
    if (iabc<ibpe) { nzmx=iabc; }
    else           { nzmx=ibpe; }
  }else{
    if (iamd<ibpe) { nzmx=iamd; }
    else           { nzmx=ibpe; }
  }
  if(nzmx<nzmi) {return cleb_ret;}
  double s1=pow(wwww,nzmi-1);
  for (int nz=nzmi; nz<=nzmx; nz++) {
    int nzm1=nz-1;
    int nzt1=iabc-nzm1;
    int nzt2=iamd-nzm1;
    int nzt3=ibpe-nzm1;
    int nzt4=nz-nzmic2;
    int nzt5=nz-nzmic3;
    double termlg=sqfclg-faclog(nz)-faclog(nzt1)-faclog(nzt2)
      -faclog(nzt3)-faclog(nzt4)-faclog(nzt5);
    double ssterm=s1*exp (termlg);
    cleb_ret=cleb_ret+ssterm;
    s1=-s1;
  }
  return cleb_ret;
}

void cleb(){
  std::cout << "hello" << std::endl;
  std::cout << "cleb(0.5,0.5,0.5,0.5,1.,1.): "
	    <<  cleb(0.5,0.5,0.5,0.5,1.,1.) << std::endl;
  std::cout << "sqrt: " << sqrt(1) << std::endl;
  std::cout << "cleb(3./2.,-1./2.,1.,1.,3./2.,1./2.): "
	    <<  cleb(3./2.,-1./2.,1.,1.,3./2.,1./2.) << std::endl;
  std::cout << "sqrt: " << -sqrt(8./15.) << std::endl;
}
