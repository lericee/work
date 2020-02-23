clear;
% statisW=zeros(30,1);
% for sta =1:30

%%%    Initialization    %%%
timer=tic;
num_par=50;
loc_Coor56=cell(num_par,4);
T_iter=50;
v_Pmax=360*0.25;
v_Amax=20;
w_up=1;
w_low=0;
c1=2;
c2=2;

pBest=cell(num_par,4); % coor5 , coor6 , weight, cross section sets
gBest=cell(1,4);  % coor5 , coor6 , weight, cross section sets
gBest_history=cell(T_iter+1,4);
%-------------parameter setting-------------%
NNOD=6;
NBC=10;
NMAT=1;
NSEC=10;
ITP=2;
NNE=2;
IFORCE=1;

COOR=zeros(2,NNOD);
NFIX=zeros(2,NNOD);
EXLD=zeros(2,NNOD);
IDBC=zeros(5,NBC);
VECTY=[];
FEF=[];
PROP=zeros(5,NMAT);
SECT=zeros(5,NSEC);

LUNIT='in';
FUNIT='kips';
%----------------matrix input----------------%
par_i=1;
while par_i<=num_par
    y5=rand*-360;
    x5=rand()*(715+y5)-y5;
    x6=rand()*(720-x5)+x5;
    while (x6-x5)<4
        x6=rand()*(720-x5)+x5;
    end
    if x5<360
       tempY=y5-(x6-x5)*(y5+360)/(720-x5);
       y6=rand()*tempY;
    else
        tY1=y5+(y5+360)/(x5-360)*(x6-x5);
        tY2=-360+(y5+360)/(720-x5)*(720-x6);
        if tY1>0
            y6=rand()*tY2;
        else
            y6=tY2+rand()*(tY1-tY2);
        end
    end
    coor5 =[x5,y5];
    coor6 =[x6,y6];
    COOR=[0 -360;360 -360;720 -360;0 0; coor5 ; coor6 ]';
    NFIX=[-1 -1;0 0;0 0;-1 -1;0 0;0 0]';
    EXLD=[0 0;0 -100000;0 -100000;0 0; 0 0;0 0]';
    IDBC=[1 2 1 1 0;1 5 1 2 0;2 4 1 3 0;2 5 1 4 0;4 5 1 5 0; ...
          2 3 1 6 0;2 6 1 7 0;3 5 1 8 0;3 6 1 9 0;5 6 1 10 0]';
    PROP=[1e7 0 0 0.1 0]';
    
    ASets=rand(1,10)*34.9+0.1;
    SECT=[ASets(1) 0 0 0 0;ASets(2) 0 0 0 0;ASets(3) 0 0 0 0;ASets(4) 0 0 0 0;ASets(5) 0 0 0 0;
          ASets(6) 0 0 0 0;ASets(7) 0 0 0 0;ASets(8) 0 0 0 0;ASets(9) 0 0 0 0;ASets(10) 0 0 0 0]';

    %------------------------%
    [d,ELFOR,M]= FRAME17(NNOD,NBC,NMAT,NSEC,ITP,NNE,IFORCE,COOR,NFIX,EXLD,IDBC,VECTY,FEF,PROP,SECT,LUNIT,FUNIT);

    if CheckConstraint(d,ELFOR,NBC,IDBC,SECT)
        loc_Coor56{par_i,1}=coor5;
        loc_Coor56{par_i,2}=coor6;
        loc_Coor56{par_i,3}=M;
        loc_Coor56{par_i,4}=ASets;
        par_i=par_i+1;
    end
end

%%%     evaluation     %%%
pBest=loc_Coor56;
k=find([pBest{:,3}]==min([pBest{:,3}]),1);
gBest=pBest(k,:);
gBest_history(1,:)=gBest;


%%%     moving particles     %%%
velocity=rand(num_par,14);
velocity(:,1:4)=velocity(:,1:4)*v_Pmax;
velocity(:,5:14)=velocity(:,5:14)*v_Amax;
for t=1:T_iter
    w=w_up-(t-1)/T_iter*(w_up-w_low);
%     v_Pmax=v_Pmax*w;
%     v_Amax=v_Amax*w;
    for j=1:num_par
        velocity(j,:)=w*velocity(j,:)+rand()*c1*([pBest{j,1}-loc_Coor56{j,1} pBest{j,2}-loc_Coor56{j,2} pBest{j,4}-loc_Coor56{j,4}]) ...
        +rand()*c2*([gBest{1}-loc_Coor56{j,1} gBest{2}-loc_Coor56{j,2} gBest{4}-loc_Coor56{j,4}] );
        
        if norm(velocity(j,1:4))>v_Pmax
            velocity(j,1:4)=velocity(j,1:4)/norm(velocity(j,1:4))*v_Pmax;
        end
        if norm(velocity(j,5:14))>v_Amax
            velocity(j,5:14)=velocity(j,5:14)/norm(velocity(j,5:14))*v_Amax;
        end
        % renew the coordinates
        coor5=loc_Coor56{j,1}+velocity(j,[1 2]);
        coor6=loc_Coor56{j,2}+velocity(j,[3 4]);
        ASets=loc_Coor56{j,4}+velocity(j,5:14);
        
        %%%  particle inside Boundary checking  %%%
        
        % coordinates 5
        while ~( (0>coor5(2)&&coor5(2)>-360) && (-coor5(2)<coor5(1)&&coor5(1)<715) )
            if coor5(2)>0
                coor5(2)=-(coor5(2)-0);                
            end
            if coor5(2)<-360
                coor5(2)=-360-(coor5(2)+360);
            end
            if coor5(1) >715
               coor5(1)=715-(coor5(1)-715); 
            end
            if coor5(1) < -coor5(2)
               distance=abs(coor5(1)+coor5(2))/sqrt(2); 
               coor5(1)=coor5(1)+2*distance*(1/sqrt(2));
               coor5(2)=coor5(2)+2*distance*(1/sqrt(2));
            end
        end
        % coordinates 6
        if coor5(1) <360
            b=720- coor5(1);
            a=-360-coor5(2);
            m=a/b;
            tY1=(coor6(1)-coor5(1))*m+coor5(2);
            while ~(( (coor5(1)+4)<coor6(1)&&coor6(1)<720) && (0>coor6(2)&&coor6(2)>tY1) )
                if coor6(1)< (coor5(1)+4)
                    coor6(1)=(coor5(1)+4)+(coor5(1)+4-coor6(1));
                end
                if coor6(1)>720
                   coor6(1)=720-(coor6(1)-720); 
                end
                if coor6(2)>0
                   coor6(2)=-coor6(2); 
                end
                tY1=(coor6(1)-coor5(1))*m+coor5(2);
                if coor6(2)<tY1
                    distance=abs((coor6(1)-coor5(1))*a+(coor5(2)-coor6(2))*b)/sqrt(a^2+b^2);
                    coor6(1)=coor6(1)+2*distance*abs(a)/sqrt(a^2+b^2);
                    coor6(2)=coor6(2)+2*distance*abs(b)/sqrt(a^2+b^2);
                end
                tY1=(coor6(1)-coor5(1))*m+coor5(2);
            end
        else 
            b1=720- coor5(1);
            a1=-360-coor5(2);
            m1=a1/b1;
            tY1=(coor6(1)-coor5(1))*m1+coor5(2);
            
            b2=(coor5(1)-360);
            a2=(coor5(2)+360);
            m2=a2/b2;
            tY2=(coor6(1)-360)*m2-360;
       
            while ~(coor6(1)<720 && coor6(1)>(coor5(1)+4)  && min(0,tY2)>=coor6(2)&&coor6(2)>=tY1)
                if coor6(1)>720
                    coor6(1)=720-(coor6(1)-720);
                end
                
                tY1=(coor6(1)-coor5(1))*m1+coor5(2);               
                tY2=(coor6(1)-360)*m2-360;                             
               
                if coor6(1)<(coor5(1)+4)
                    coor6(1)=coor5(1)+4+abs(coor6(1)-coor5(1)-4);
                end
                
                tY1=(coor6(1)-coor5(1))*m1+coor5(2);               
                tY2=(coor6(1)-360)*m2-360;  
                
                if coor6(2)<tY1
                    distance=abs((coor6(1)-coor5(1))*a1+(coor5(2)-coor6(2))*b1)/sqrt(a1^2+b1^2);
                    coor6(1)=coor6(1)+2*distance*abs(a1)/sqrt(a1^2+b1^2);
                    coor6(2)=coor6(2)+2*distance*abs(b1)/sqrt(a1^2+b1^2);

                end
                
                tY1=(coor6(1)-coor5(1))*m1+coor5(2);            
                tY2=(coor6(1)-360)*m2-360;
                
                if coor6(2) > min(tY2,0)
                    if tY2 < 0
                        %  mistake
                       distance=abs((coor6(1)-coor5(1))*a2+(coor5(2)-coor6(2))*b2)/sqrt(a2^2+b2^2);
                       coor6(1)=coor6(1)+2*distance*abs(a2)/sqrt(a2^2+b2^2);
                       coor6(2)=coor6(2)-2*distance*abs(b2)/sqrt(a2^2+b2^2);

                    else
                       coor6(2)=-coor6(2); 
                    end
                end
              
                tY1=(coor6(1)-coor5(1))*m1+coor5(2);        
                tY2=(coor6(1)-360)*m2-360;
            end    
        end        
        %%%-------------------------------------%%%
                
        COOR(:,5)=coor5';
        COOR(:,6)=coor6';
        
        
        % -------- Area constraints -------- %
        for s=1:length(ASets) 
            while 0.1>ASets(s) || 35<ASets(s)
                if 0.1>ASets(s)
                    ASets(s) =0.1+0.1-ASets(s);
                end
                if 35<ASets(s)
                    ASets(s) =35-(ASets(s)-35);
                end
            end
                
%             if ASets(s) <0.1
%                 ASets(s)=0.1;
%             end
%             if ASets(s)>35
%                 ASets(s)=35;
%             end
        end
        
        SECT=[ASets(1) 0 0 0 0;ASets(2) 0 0 0 0;ASets(3) 0 0 0 0;ASets(4) 0 0 0 0;ASets(5) 0 0 0 0;
              ASets(6) 0 0 0 0;ASets(7) 0 0 0 0;ASets(8) 0 0 0 0;ASets(9) 0 0 0 0;ASets(10) 0 0 0 0]';
        % ---------------------------------- %
        
        [d,ELFOR,M]= FRAME17(NNOD,NBC,NMAT,NSEC,ITP,NNE,IFORCE,COOR,NFIX,EXLD,IDBC,VECTY,FEF,PROP,SECT,LUNIT,FUNIT);
    
        if CheckConstraint(d,ELFOR,NBC,IDBC,SECT)
            loc_Coor56{j,1}=coor5;
            loc_Coor56{j,2}=coor6;
            loc_Coor56{j,3}=M;
            loc_Coor56{j,4}=ASets;
        end
        if loc_Coor56{j,3} < pBest{j,3}
           pBest(j,:) =loc_Coor56(j,:);
        end
    
    end
    k=find([pBest{:,3}]==min([pBest{:,3}]),1);
    gBest=pBest(k,:);
    gBest_history(t+1,:)=gBest;
    
end
fprintf('elasped time : %.4f\n',toc(timer));
COOR=[0 -360;360 -360;720 -360;0 0; gBest{1} ; gBest{2} ]';
FORMAT='black';
figure();
drawingStructure(ITP,COOR,IDBC,NBC,LUNIT,FORMAT);
figure();
plot(1:T_iter+1,[gBest_history{:,3}]);
fprintf('Min Weight: %.3f  lbf\n',gBest{3})
fprintf('Coordinates of node 5:\n x: %.3f , y: %.3f\n',gBest{1});
fprintf('Coordinates of node 6:\n x: %.3f , y: %.3f\n',gBest{2});
fprintf('Area of elements: \n');
disp(gBest{4});

% statisW(sta)=gBest{3};
% end
% disp(mean(statisW));
% disp(std(statisW));

function bool=CheckConstraint(d,ELFOR,NBC,IDBC,SECT)
    for j=1:length(d)
       if (d(j)>=2) || (d(j)<=-2)
           bool=0;
           return;
       end
    end
    for j=1:NBC
       if (ELFOR(1,j)/SECT(1,IDBC(4,j)) >= 25000) || (ELFOR(1,j)/SECT(1,IDBC(4,j))<= -25000)
           bool= 0;
           return
       end
    end
    bool =1;
end