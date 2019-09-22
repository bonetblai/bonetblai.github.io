%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% 
%%% wrapper class for beamer with some layout defaults
%%% and a "4-on-1" option for handouts
%%%
%%% Copyright: Malte Helmert
%%% All rights reserved 
%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\NeedsTeXFormat{LaTeX2e} 
\ProvidesClass{gkibeamer}[2006/10/30 v0.1 beamer wrapper class]
\RequirePackage{ifthen}

\newboolean{hand@out}
\newboolean{four@on@one}

\DeclareOption{handout}{
  \setboolean{hand@out}{true}
  \PassOptionsToClass{handout}{beamer}
}
\DeclareOption{4-on-1}{
  \setboolean{four@on@one}{true}
}
\DeclareOption*{
  \PassOptionsToClass{\CurrentOption}{beamer}
}
\ProcessOptions
\LoadClass{beamer}

\def\lecture@no{}
\newcommand{\lectureno}[1]{\edef\lecture@no{#1}}
\newcommand{\the@lectureno}{%
  \ifthenelse{\equal{\lecture@no}{}}{}{\lecture@no.}}

\iffour@on@one
  \RequirePackage{pgf}
  \RequirePackage{pgfpages}

  \definecolor{pagebordercolor}{rgb}{0.129216,0.188235,0.709804}

  \newcounter{@remainder}%
  \newcommand{\set@remainder}[2]{%
    \setcounter{@remainder}{#1}%
    \loop%
    \ifnum\the@remainder<#2\relax%
    \else%
    \addtocounter{@remainder}{-#2}%
    \repeat%
  }

  \newcounter{pageno@temp}
  \newcommand{\border@ifnecessary}[1]{%
    \setcounter{pageno@temp}{\thepage}%
    \addtocounter{pageno@temp}{2}%
    \set@remainder{\thepageno@temp}{4}%
    \addtocounter{@remainder}{2}%
    \ifnum#1<\the@remainder\relax%
    \color{pagebordercolor}\pgfsetlinewidth{2pt}\pgfstroke%
    \fi%
  }

  \pgfpagesdeclarelayout{my@pagelayout}
  {
    \edef\pgfpageoptionheight{\the\paperheight} 
    \edef\pgfpageoptionwidth{\the\paperwidth}
    \edef\pgfpageoptionborder{0pt}
  }
  {
    \pgfpagesphysicalpageoptions
    {%
      logical pages=4,%
      physical height=\pgfpageoptionheight,%
      physical width=\pgfpageoptionwidth%
    }
    \pgfpageslogicalpageoptions{1}
    {%
      border code=\border@ifnecessary{1},%
      border shrink=\pgfpageoptionborder,%
      resized width=.5\pgfphysicalwidth,%
      resized height=.5\pgfphysicalheight,%
      center=\pgfpoint{.25\pgfphysicalwidth}{.75\pgfphysicalheight}%
    }%
    \pgfpageslogicalpageoptions{2}
    {%
      border code=\border@ifnecessary{2},%
      border shrink=\pgfpageoptionborder,%
      resized width=.5\pgfphysicalwidth,%
      resized height=.5\pgfphysicalheight,%
      center=\pgfpoint{.75\pgfphysicalwidth}{.75\pgfphysicalheight}%
    }%
    \pgfpageslogicalpageoptions{3}
    {%
      border code=\border@ifnecessary{3},%
      border shrink=\pgfpageoptionborder,%
      resized width=.5\pgfphysicalwidth,%
      resized height=.5\pgfphysicalheight,%
      center=\pgfpoint{.25\pgfphysicalwidth}{.25\pgfphysicalheight}%
    }%
    \pgfpageslogicalpageoptions{4}
    {%
      border code=\border@ifnecessary{4},%
      border shrink=\pgfpageoptionborder,%
      resized width=.5\pgfphysicalwidth,%
      resized height=.5\pgfphysicalheight,%
      center=\pgfpoint{.75\pgfphysicalwidth}{.25\pgfphysicalheight}%
    }%
  }

  \pgfpagesuselayout{my@pagelayout}[a4paper,landscape,border shrink=5mm]
\fi


\ifhand@out
  \newcommand{\maketitles}{
    \let\origsubtitle\insertsubtitle
    \subtitle{\the@lectureno\ \origsubtitle}
    \begin{frame}
      \titlepage
    \end{frame}
    \begin{frame}
      \frametitle{\inserttitle}
      \framesubtitle{{\insertdate} --- {\insertsubtitle}}
      \tableofcontents[subsectionstyle=hide]
    \end{frame}
  }
  \useoutertheme{infolines}
  %% footline modified from definition in infolines theme
  %% (replaced frame numbers with page numbers)
  \setbeamertemplate{footline}{
    \leavevmode%
    \hbox{%
      \begin{beamercolorbox}[wd=.333333\paperwidth,ht=2.25ex,dp=1ex,center]{author in head/foot}%
        \usebeamerfont{author in head/foot}\insertshortauthor~~(\insertshortinstitute)
      \end{beamercolorbox}%
      \begin{beamercolorbox}[wd=.333333\paperwidth,ht=2.25ex,dp=1ex,center]{title in head/foot}%
        \usebeamerfont{title in head/foot}\insertshorttitle
      \end{beamercolorbox}%
      \begin{beamercolorbox}[wd=.333333\paperwidth,ht=2.25ex,dp=1ex,right]{date in head/foot}%
        \usebeamerfont{date in head/foot}\insertshortdate{}\hspace*{2em}
        \insertpagenumber{} / \insertpresentationendpage\hspace*{2ex} 
    \end{beamercolorbox}}%
    \vskip0pt%
  }
  \defbeamertemplate{section in toc}{our style}
  {\leavevmode\begin{Large}\hilite{\the@lectureno\inserttocsectionnumber\ \inserttocsection}\end{Large}\par}

  \defbeamertemplate{subsection in toc}{our style}
  {\leavevmode\leftskip=5ex\llap{%
      \usebeamercolor[bg]{subsection number projected}%
          \vrule width1ex height1ex\kern1ex}\inserttocsubsection\par}
  \setbeamertemplate{sections/subsections in toc}[our style]

\else
  \newcommand{\maketitles}{%
    \let\origsubtitle\insertsubtitle
    \subtitle{\the@lectureno\ \origsubtitle}%
    \maketitle}
  \usetheme[right,hideothersubsections]{PaloAlto}
  \usecolortheme[rgb={0.0,0.34,0.37}]{structure}
  \usefonttheme{professionalfonts}
  \setbeamercovered{transparent}
  \setbeamertemplate{navigation symbols}{}
  \setbeamertemplate{sidebar canvas right}[vertical shading][bottom=black,top=palette primary.bg]
  \setbeamertemplate{section in toc shaded}[default][60]
  \setbeamercolor{logo}{bg=palette primary.bg}
\fi

\institute[Universit\"at Freiburg]{Albert-Ludwigs-Universit\"at Freiburg}
\date{}
\AtBeginSection[]
{
    \begin{frame}<beamer>
        \begin{Huge}\hilite{\insertsection}\end{Huge}
    \end{frame}
    \begin{frame}<handout>
        \frametitle{\the@lectureno\thesection\ \insertsection}
        \tableofcontents[sectionstyle=hide, hideothersubsections]
    \end{frame}
}
