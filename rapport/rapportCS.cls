%B65;6800;1c
\LoadClass[a4paper,12pt]{article} % Classe du document

%--------------------- Packages ------------------------

\RequirePackage[french]{babel} %Langue du document
\RequirePackage[utf8]{inputenc} %Caractères spéciaux
%\RequirePackage[section]{placeins}%Pour placement de section
\RequirePackage[T1]{fontenc} %Quelques lettres qui sont pas inclus dans UTF-8
\RequirePackage{mathtools} %Paquet pour des équations et symboles mathématiques
%\RequirePackage{siunitx} %Pour écrire avec la notation scientifique (Ex.: \num{2e+9})
\RequirePackage{float} %Pour placement d'images
\RequirePackage{graphicx} %Paquet pour insérer des images
\RequirePackage[justification=centering]{caption} %Pour les légendes centralisées
\RequirePackage{subcaption}
%\RequirePackage{wallpaper}
%\RequirePackage{nomencl}
%\makenomenclature
\RequirePackage{fancyhdr}
%\RequirePackage{lastpage}
%\pagestyle{fancy}
%\fancyheadoffset{1cm}
%\setlength{\headheight}{2cm}
\RequirePackage{url}
\RequirePackage[hidelinks]{hyperref}%Paquet pour insérer légendes dans des sous-figures comme Figure 1a, 1b
\RequirePackage[left=2.5cm,right=2cm,top=2cm,bottom=3cm]{geometry} %Configuration de la page
\RequirePackage{enumitem}

%\renewcommand\thesection{\Roman{section}}
%\renewcommand\thesubsection{\hspace{1em}\arabic{subsection}}

%\RequirePackage{titlesec}

\setcounter{secnumdepth}{4}

%\titleformat{\paragraph}
%{\normalfont\normalsize\bfseries}{\theparagraph}{1em}{}
%\titlespacing*{\paragraph}
%{1.5em}{3.25ex plus 1ex minus .2ex}{1.5ex plus .2ex}

\renewcommand\theparagraph{\hspace{1em}\alph{paragraph}}

\newcommand{\code}[1]{\texttt{#1}}

%-------------------- Informations sur le rapport ----------------------

\newcommand{\sujetprojet}[1]{\renewcommand{\sujetprojet}{#1}}
\newcommand{\lieuprojet}[1]{\renewcommand{\lieuprojet}{#1}}
\newcommand{\basdepage}[1]{\renewcommand{\basdepage}{#1}}

\newcommand{\titre}[1]{\renewcommand{\titre}{#1}}

\newcommand{\eleve}[1]{\renewcommand{\eleve}{#1}}

\newcommand{\ensref}[1]{\renewcommand{\ensref}{#1}}
\newcommand{\tuteurentreprise}[1]{\renewcommand{\tuteurentreprise}{#1}}

\newcommand{\logoentreprise}[1]{\renewcommand{\logoentreprise}{#1}}

\newcommand{\dates}[1]{\renewcommand{\dates}{#1}}
\newcommand{\shortdates}[1]{\renewcommand{\shortdates}{#1}}

\newcommand{\fairemarges}{
%\makenomenclature
\pagestyle{fancy}
\fancyheadoffset{0.5cm}


\lhead{\includegraphics[scale=0.03]{logos/logoecole.png}\hspace{0.5em}}
\chead{\shortdates}
\rhead{\nouppercase{\leftmark}}
\rfoot{Page \thepage}
%\rfoot{Page \thepage/\pageref{LastPage}}
\cfoot{\textbf{\basdepage}}
\lfoot{\lieuprojet}

}

\newcommand{\fairepagedegarde}{
\begin{titlepage}
%\ThisURCornerWallPaper{0.5}{logos/URC-gfx.png}
%\ThisLLCornerWallPaper{0.4}{logos/LLC-gfx.png}

\centering %Centraliser le contenu

% Logo Entreprise x prepa
\begin{figure}
    \begin{subfigure}{.5\textwidth}
        \centering
        \includegraphics[width=1\textwidth]{logos/logoecole.png}\par\vspace{1cm} %Insertion du logo prépa
    \end{subfigure}
\end{figure}

\vspace{10cm}
% Titre du rapport
\rule{\linewidth}{0.2 mm} \\[0.4 cm]
{\huge\bfseries \titre \par} \
\rule{\linewidth}{0.2 mm} \\[1.0 cm]

% Nom de l'étudiant
{\scshape\LARGE \eleve \par} 
\vspace{1.0cm}

% Parcours de l'étudiant
{\scshape Sujet : \sujetprojet} 
\vspace{0.5cm}

% Dates
{\large \dates \par} 
	

\end{titlepage}
\newpage
}

\newcommand{\tabledematieres}{
\tableofcontents
\newpage
}

\newcommand{\insererfigure}[4]{
\begin{figure}[!ht]
\centering
\includegraphics[height=#2]{#1}
\caption{#3}
\label{fig: #4}
\end{figure}
}



